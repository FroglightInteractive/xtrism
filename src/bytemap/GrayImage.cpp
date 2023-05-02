// GrayImage.C

#include <stdlib.h>
#include "GrayImage.h"
#include "../basics/minmax.h"
#include "../basics/dbx.h"

GrayImage::GrayImage(unsigned int w, unsigned int h):
  store(w, h, QImage::Format_Grayscale8) {
  dat = store.bits();
  wid = store.width();
  hei = store.height();
  bpl = store.bytesPerLine();
}

GrayImage::GrayImage(unsigned int w, unsigned int h, unsigned char ini):
  store(w, h, QImage::Format_Grayscale8) {
  store.fill(ini);
  dat = store.bits();
  wid = store.width();
  hei = store.height();
  bpl = store.bytesPerLine();
}


/* Boiler plate copy/move code fobplows */
GrayImage::GrayImage(GrayImage const &oth) {
  store = oth.store;
  store.detach();
  oth.dat = oth.store.bits(); // needed?
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

GrayImage &GrayImage::operator=(GrayImage const &oth) {
  store = oth.store;
  store.detach();
  oth.dat = oth.store.bits(); // needed?
  dat = store.bits();
  wid = store.width();
  hei = store.height();
  bpl = store.bytesPerLine();
  return *this;
}

GrayImage &GrayImage::operator=(GrayImage &&oth) {
  store = std::move(oth.store);
  dat = store.bits();
  wid = store.width();
  hei = store.height();
  bpl = store.bytesPerLine();
  return *this;
}
/* End of boiler plate copy/move code */

GrayImage GrayImage::fromQImage(QImage const &img) {
  GrayImage out(0,0);
  out.store = img.convertToFormat(QImage::Format_Grayscale8);
  out.store.detach();
  out.dat = out.store.bits();
  out.wid = out.store.width();
  out.hei = out.store.height();
  out.bpl = out.store.bytesPerLine();
  return out;
}  

GrayImage GrayImage::fromFile(QString filename) {
  GrayImage out(0,0);
  out.store.load(filename);
  out.store.convertTo(QImage::Format_Grayscale8);
  out.dat = out.store.bits();
  out.wid = out.store.width();
  out.hei = out.store.height();
  out.bpl = out.store.bytesPerLine();
  return out;
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

//////////////////////////////////////////////////////////////////////
inline void GrayImage::recolorPix(int x, int y, float mul, int add) {
  unsigned char &c(pix(x, y));
  c = clip255(mul*c + add);
}

void GrayImage::recolorRect(BBox const &bbox, float mul, int add) {
  int x0 = bbox.left();
  int y0 = bbox.top();
  int x1 = x0 + bbox.width();
  int y1 = y0 + bbox.height();
  Q_ASSERT_X(x0>=0 && y0>=0 && x1<=wid && y1<=hei, "recolorRect", "too big");
  for (int y=y0; y<y1; y++)
    for (int x=x0; x<x1; x++)
      recolorPix(x, y, mul, add);
}
  
void GrayImage::recolorRectEdges(BBox const &bbox, int bw, int depth) {
  int x0 = bbox.left();
  int y0 = bbox.top();
  int w = bbox.width();
  int h = bbox.height();
  int x1 = x0 + w - 1;
  int y1 = y0 + h - 1;
  Q_ASSERT_X(w >= 2*bw && h >= 2*bw, "recolorRectEdges", "too small");
  Q_ASSERT_X(x0>=0 && y0>=0 && x1<wid && y1<hei, "recolorRectEdges", "too big");
  for (int y = 1; y <= bw; y++) 
    for (int x = 0; x < w - y; x++) 
      recolorPix(x + x0, y + y0 - 1, 1., depth);
  for (int y = 1; y <= bw; y++) 
    for (int x = 0; x < w - y; x++) 
      recolorPix(x1 - x, y1 + 1 - y, 1., -depth);
  for (int x = 1; x <= bw; x++)
    for (int y = bw; y < h - x; y++) 
      recolorPix(x + x0 - 1, y + y0, 1., depth);
  for (int x = 1; x <= bw; x++)
    for (int y = bw; y < h - x; y++) 
      recolorPix(x1 - x + 1, y1 - y, 1., -depth);
}
  
void GrayImage::recolorCircle(QPoint const &center,
                              int radius, float mul, int add,
                              int r1, int depth) {
  int rad2 = radius * radius;
  int r12 = r1 * r1;
  int cx = center.x();
  int cy = center.y();
  Q_ASSERT_X(cx-r1>=0 && cy-r1>=0 && cx+r1<wid && cy+r1<hei && radius<=r1,
             "recolorCircle", "too big");
  for (int y = -r1; y <= r1; y++) {
    int y2 = y * y;
    // int bdx=int(sqrt(rad2-y2));
    int bdx1 = int(sqrt(r12 - y2));
    for (int x = -bdx1; x <= bdx1; x++) {
      int x2 = x * x;
      int r2 = x2 + y2;
      if (r2 <= rad2)
        recolorPix(cx + x, cy + y, mul, add);
      else
        recolorPix(cx + x, cy + y, 1., -depth * (y + x) / sqrt(r2));
    }
  }
}  
