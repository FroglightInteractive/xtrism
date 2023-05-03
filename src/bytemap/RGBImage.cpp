// RGBImage.C

#include "RGBImage.h"
#include "GrayImage.h"
#include "minmax.h"

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

constexpr float X0 = 0.5;
constexpr float Y0 = 0.5;
constexpr float DECAY = 2. / 3;
constexpr float GREY0 = 0;
constexpr float dGREY = 1.5;

RGBImage RGBImage::colorized(QSize size, int bw, QRectF rainbow) {
  BrickCell bc(size.width(), size.height());
  if (bw)
    bc.drawborders(bw, BCSurround());
  return colorized(bc, rainbow);
}

RGBImage RGBImage::colorized(GrayImage const &src, QRectF rainbow) {
  float left = rainbow.left();
  float width = rainbow.width();
  float top = rainbow.top();
  float height = rainbow.height();
  left -= X0;
  top -= Y0;
  int w = src.width();
  int h = src.height();
  RGBImage out(w,h);
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      float phi = atan2(left + width * x / w, -top - height * y / h);
      phi -= PI / 6;
      if (phi < -PI)
        phi += 2 * PI;
      float green = std::max(std::cos(phi * DECAY), 0.f);
      phi -= 2 * PI / 3;
      if (phi < -PI)
        phi += 2 * PI;
      float red = std::max(std::cos(phi * DECAY), 0.f);
      phi -= 2 * PI / 3;
      if (phi < -PI)
        phi += 2 * PI;
      float blue = std::max(std::cos(phi * DECAY), 0.f);
      float grey = src.get(x, y) * dGREY + GREY0;
      out.rset(x, y, grey * red, grey * green, grey * blue);
    }
  }
  return out;
}
