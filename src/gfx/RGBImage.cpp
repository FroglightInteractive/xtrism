// RGBImage.C

#include "RGBImage.h"
#include "GrayImage.h"
#include "minmax.h"
#include <cmath>
#include <QTextStream>
#include <QFile>

RGBImage::RGBImage(unsigned int w, unsigned int h):
  store(w, h, QImage::Format_RGB32) {
  dat = (uint*)store.bits();
  wid = store.width();
  hei = store.height();
  wpl = store.bytesPerLine()/4;
}

RGBImage::RGBImage(unsigned int w, unsigned int h, uint ini):
  store(w, h, QImage::Format_RGB32) {
  store.fill(ini);
  dat = (uint*)store.bits();
  wid = store.width();
  hei = store.height();
  wpl = store.bytesPerLine()/4;
}


/* Boiler plate copy/move code fowplows */
RGBImage::RGBImage(RGBImage const &oth) {
  store = oth.store;
  store.detach();
  oth.dat = (uint*)oth.store.bits(); // needed?
  dat = (uint*)store.bits();
  wid = store.width();
  hei = store.height();
  wpl = store.bytesPerLine()/4;
}

RGBImage::RGBImage(RGBImage &&oth) {
  store = std::move(oth.store);
  dat = (uint*)store.bits();
  wid = store.width();
  hei = store.height();
  wpl = store.bytesPerLine()/4;
}

RGBImage &RGBImage::operator=(RGBImage const &oth) {
  store = oth.store;
  store.detach();
  oth.dat = (uint*)oth.store.bits(); // needed?
  dat = (uint*)store.bits();
  wid = store.width();
  hei = store.height();
  wpl = store.bytesPerLine()/4;
  return *this;
}

RGBImage &RGBImage::operator=(RGBImage &&oth) {
  store = std::move(oth.store);
  dat = (uint*)store.bits();
  wid = store.width();
  hei = store.height();
  wpl = store.bytesPerLine()/4;
  return *this;
}
/* End of boiler plate copy/move code */

RGBImage RGBImage::fromQImage(QImage const &img) {
  RGBImage out(0,0);
  out.store = img.convertToFormat(QImage::Format_RGB32);
  out.store.detach();
  out.dat = (uint*)out.store.bits();
  out.wid = out.store.width();
  out.hei = out.store.height();
  out.wpl = out.store.bytesPerLine()/4;
  return out;
}  

RGBImage RGBImage::fromFile(QString filename) {
  RGBImage out(0,0);
  out.store.load(filename);
  out.store.convertTo(QImage::Format_RGB32);
  out.dat = (uint*)out.store.bits();
  out.wid = out.store.width();
  out.hei = out.store.height();
  out.wpl = out.store.bytesPerLine()/4;
  return out;
}

RGBImage RGBImage::cropped(unsigned int x, unsigned int y,
                           unsigned int w, unsigned int h) const {
  return fromQImage(store.copy(x, y, w, h));
}

RGBImage RGBImage::rotated(int rot) const {
  if (rot&3 == 0)
    return *this;
  Q_ASSERT_X(hei==wid, "rotated", "Cannot rotate non-square image");
  RGBImage out(hei, wid);
  switch (rot & 3) {
  case 1:
    for (unsigned int y = 0; y < hei; y++)
      for (unsigned int x = 0; x < wid; x++)
        out.pix(x, y) = get(y, wid - 1 - x);
    break;
  case 2:
    for (unsigned int y = 0; y < hei; y++)
      for (unsigned int x = 0; x < wid; x++)
        out.pix(x, y) = get(wid - 1 - x, hei - 1 - y);
    break;
  case 3:
    for (unsigned int y = 0; y < hei; y++)
      for (unsigned int x = 0; x < wid; x++)
        out.pix(x, y) = get(wid - 1 - y, x);
    break;
  }
  return out;
}

QImage const &RGBImage::toQImage() const {
  return store;
}

void RGBImage::save(QString filename) const {
  toQImage().save(filename);
}

RGBImage RGBImage::colorized(class GrayImage const &src,
                             WideRGB const &rgb0, WideRGB const &rgb1) {
  uint table[256];
  int w = src.width();
  int h = src.height();
  for (int gray=0; gray<256; gray++) {
    float g1 = gray/255.0;
    float g0 = 1 - g1;
    table[gray] = qRgb(clip255(g0*rgb0.r + g1*rgb1.r),
                       clip255(g0*rgb0.g + g1*rgb1.g),
                       clip255(g0*rgb0.b + g1*rgb1.b));
    // table[gray] = qRgb(gray, gray, gray);
  }
  RGBImage out(w, h);
  for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++)
      out.set(x, y, table[src.get(x, y)]);
  return out;
}

//////////////////////////////////////////////////////////////////////
// MarbleBG.C

#include "BrickCell.h"

#include <cmath>
#ifndef PI
#define PI 3.14159265358979323846               /* pi */
#endif


RGBImage RGBImage::colorized(QSize size, int bw, QRectF rainbow) {
  BrickCell bc(size.width(), size.height());
  if (bw)
    bc.drawborders(bw, BCSurround());
  return colorized(bc, rainbow);
}

struct FloatRGB {
public:
  FloatRGB(float r=0, float g=0, float b=0): r(r), g(g), b(b) { }
  float r, g, b;
};

static QVector<FloatRGB> dhsv(int N=1024) {
  int L = 6*N;
  QVector<FloatRGB> rgb0(6*N);
  auto up = [N](int n) {
    return .5*(.5 - .5*std::cos((.5+n)*PI/N)) + .5*(n+.5)/N;
  };
  auto dn = [N, up](int n) {
    return up(N-1-n);
  };
  for (int n=0; n<N; n++) {
    rgb0[n]     = FloatRGB(1, up(n), 0);
    rgb0[N+n]   = FloatRGB(dn(n), 1, 0);
    rgb0[2*N+n] = FloatRGB(0, 1, up(n));
    rgb0[3*N+n] = FloatRGB(0, dn(n), 1);
    rgb0[4*N+n] = FloatRGB(up(n), 0, 1);
    rgb0[5*N+n] = FloatRGB(1, 0, dn(n));
  }

  QVector<float> ll(L);
  for (int k=0; k<L; k++)
    ll[k] = k*360.0/L;

  auto adjusthue = [L, &ll](float l0, float dl, float sc) {
    auto sqr = [](float x) { return x*x; };
    auto dL = [L, l0, dl, sc, sqr](float l) {
      return 1 - sc*(std::exp(-sqr((l-l0)/dl))
                     + std::exp(-sqr((l-360-l0)/dl))
                     + std::exp(-sqr((l+360-l0)/dl)));
    };
    float cs = 0;
    for (int k=0; k<L; k++) {
      cs += dL(ll[k]);
      ll[k] = cs;
    }
    for (int k=0; k<L; k++)
      ll[k] = 360*ll[k] / cs;
    for (int idx=0; idx<L; idx++) {
      if (ll[idx] >= l0) {
        float li = ll[idx];
        for (int k=0; k<L; k++)
          ll[k] = std::fmod(ll[k] - li + l0 + 360, 360);
        break;
      }
    }
  };

  auto interp1 = [L](QVector<FloatRGB> const &in, QVector<float> const &ll) {
    QVector<FloatRGB> out(L);
    for (int k=0; k<L; k++) {
      int n = L*ll[k]/360;
      out[k] = in[n];
    }
    return out;
  };

  adjusthue(320, 30, -0.8);
  QVector<FloatRGB> rgb1 = interp1(rgb0, ll);
  adjusthue(240, 60, 0.45);
  rgb0 = interp1(rgb1, ll);
  adjusthue(245, 25, 0.35);
  rgb1 = interp1(rgb0, ll);
  for (int k=0; k<L; k++)
    ll[k] = std::fmod(ll[k] + 180, 360);
  adjusthue(180, 30, .65);
  rgb0 = interp1(rgb1, ll);

  auto lin2srgb = [](float c) {
    if (c>.0031308)
      return std::pow(1.055*c, 1/2.4) - .055;
    else
      return c * 12.92;
  };
  for (int k=0; k<L; k++)
    rgb1[k] = FloatRGB(lin2srgb(rgb0[k].r),
                       lin2srgb(rgb0[k].g),
                       lin2srgb(rgb0[k].b));
  QFile fd("/tmp/rgb.txt");
  fd.open(QFile::WriteOnly);
  QTextStream ts(&fd);
  for (int k=0; k<L; k++)
    ts << "[" << rgb1[k].r << "," << rgb1[k].g << "," << rgb1[k].b << "],\n";
  return rgb1;
}

static QVector<FloatRGB> lut(dhsv());
 

RGBImage RGBImage::colorized(GrayImage const &src, QRectF rainbow) {
  float left = rainbow.left();
  float width = rainbow.width();
  float top = rainbow.top();
  float height = rainbow.height();
  left -= .5;
  top -= .5;
  int w = src.width();
  int h = src.height();
  RGBImage out(w,h);
  int L = lut.size();
  for (int y = 0; y < h; y++) {
    float y1 = -top - height * y / h;
    for (int x = 0; x < w; x++) {
      float x1 = left + width * x /w;
      float phi = (y1>.15) 
        ? 31.3 + 3*atan2(x1, y1-.1)
        : 18.1 - 2*atan2(x1, y1-.7);
//      float phi = (y1>.15) 
//        ? 31.3 + 3*atan2(x1, y1-.1)
//        : 21.2 - atan2(x1, y1-.2);
      int l = int(phi*L/(2*PI) + L) % L;
      float grey = src.get(x, y) * 1.5 + 0.0;
      out.rset(x, y, grey * lut[l].r, grey * lut[l].g, grey * lut[l].b);
    }
  }
  return out;
}
