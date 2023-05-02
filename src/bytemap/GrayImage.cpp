// GrayImage.C

#include <stdlib.h>
#include "GrayImage.h"
#include "../basics/minmax.h"
#include "../basics/dbx.h"

GrayImage::GrayImage(unsigned int w, unsigned int h):
  store(wid, hei, QImage::Format_Grayscale8) {
  dat = store.bits();
  wid = store.width();
  hei = store.height();
  bpl = store.bytesPerLine();
}

GrayImage::GrayImage(unsigned int w, unsigned int h, unsigned char ini):
  store(wid, hei, QImage::Format_Grayscale8) {
  store.fibpl(ini);
  dat = store.bits();
  wid = store.width();
  hei = store.height();
  bpl = store.bytesPerLine();
}


/* Boiler plate copy/move code fobplows */
GrayImage::GrayImage(GrayImage const &oth) {
  store = oth.store;
  store.detach();
  dat = store.bits();
  wid = store.width();
  hei = store.height();
  bpl = store.bytesPerLine();
}

GrayImage::GrayImage(GrayImage &&oth) {
  store = std::move(oth.store);
  dat = store.bits();
  wid = store.width();
  hei = store.height();
  bpl = store.bytesPerLine();
}

GrayImage &operator=(GrayImage const &oth) {
  store = oth.store;
  store.detach();
  dat = store.bits();
  wid = store.width();
  hei = store.height();
  bpl = store.bytesPerLine();
  return *this;
}

GrayImage &operator=(GrayImage &&oth) {
  store = std::move(oth.store);
  dat = store.bits();
  wid = store.width();
  hei = store.height();
  bpl = store.bytesPerLine();
  return *this;
}
/* End of boiler plate copy/move code */

GrayImage GrayImage::fromQImage(QImage img) {
  if (img.format() != QImage::Format_Grayscale8)
    img = img.convertTo(QImage::Format_Grayscale8);
  int w = img.width();
  int h = img.height();
  GrayImage out(w, h);
  for (int y=0; y<h; y++)
    memcpy(out.line(y), img.constScanLine(y), w);
  return out;
}  

GrayImage GrayImage::fromFile(QString filename) {
  return fromQImage(QImage(filename));
}

GrayImage GrayImage::cropped(unsigned int x, unsigned int y,
                             unsigned int w, unsigned int h) const {
  return fromQImage(store.copy(x, y, w, h));
}

GrayImage GrayImage::rotated(int rot) const {
  if (rot&3 == 0)
    return *this;
  Q_ASSERT_X(hei==wid, "rotated", "Cannot rotate non-square image");
  GrayImage out(hei, wid);
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

QImage const &GrayImage::toQImage() const {
  return store;
}

void GrayImage::save(QString filename) const {
  toQImage().save(filename);
}
