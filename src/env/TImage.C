// TImage.C

#define TPRIVATE

#include "TImage.H"
#include "TEnv.H"
#include <stdlib.h>
#include "../basics/dbx.H"

#include "../basics/BBox.H"

TImage::TImage(const TEnv &tenv, unsigned int w, unsigned int h):
  env(tenv), priv(1) {
  dbx(3, "TImage::TImage (w,h)=(%i,%i)", w, h);
  int sp = env.scanlpad();
  int scanunitpl = (w * env.bpp() + sp - 1) / sp;
  int bpl = scanunitpl * (sp / 8); // bytes per line;
  dbx(3, "TIm: depth=%i, bpp=%i, sp=%i bpl=%i", env.depth(),
      env.bpp(), sp, bpl);
  char *data = (char *)malloc(bpl * h);
  if (!data)
    athrow("Memory failure");
  image = XCreateImage(env.disp(), env.vis(), env.depth(),
                       ZPixmap, 0, data,
                       w, h, sp, bpl);
  if (!image)
    athrow("Image creation failed");
}

TImage::TImage(const TEnv &tenv, XImage *xim, bool delafter):
  env(tenv), image(xim), priv(delafter) {
}

TImage::~TImage() {
  if (priv)
    XDestroyImage(image);
}

unsigned int TImage::width() const {
  return image->width;
}
unsigned int TImage::height() const {
  return image->height;
}
unsigned int TImage::depth() const {
  return image->depth;
}

void TImage::put(int x, int y) const {
  XPutImage(env.disp(), env.win(), env.defaultgc(), image,
            0, 0, x, y, width(), height());
}

void TImage::partput(class BBox const &from, class Point const &to) const {
  XPutImage(env.disp(), env.win(), env.defaultgc(), image,
            from.left(), from.top(),
            to.x(), to.y(),
            from.width(), from.height());
}

inline unsigned int rounddown(unsigned int x, unsigned int mod) {
  return x - (x % mod);
}

void TImage::tile(BBox const &dest) const {
  int w = width(), h = height();
  int x0 = rounddown(dest.left(), w), y0 = rounddown(dest.top(), h);
  for (int y = y0; y < dest.bottom(); y += h)
    for (int x = x0; x < dest.right(); x += w) {
      BBox b(x, y, x + w, y + h);
      b &= dest;                      // destination rectangle
      BBox c(b);
      c.shift(-x, -y);             // source rectangle
      partput(c, b.topleft());
    }
}