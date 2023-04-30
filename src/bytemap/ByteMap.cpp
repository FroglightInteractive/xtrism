// ByteMap.C

#include <stdlib.h>
#include "ByteMap.h"
#include "../basics/minmax.h"
#include "../basics/dbx.h"

ByteMap::ByteMap(unsigned int w, unsigned int h, unsigned char ini):
  wid(w), hei(h), store(wid*hei, ini) {
  dat = store.data();
}

ByteMap::ByteMap(QString filename) {
  QImage img(filename);
  if (img.format() != QImage::Format_Grayscale8)
    img = img.convertToFormat(QImage::Format_Grayscale8);
  wid = img.width();
  hei = img.height();
  store.resize(wid*hei);
  dat = store.data();
  for (int y=0; y<hei; y++)
    memcpy(line(y), img.constScanLine(y), wid);
}

ByteMap::~ByteMap() {
}

ByteMap::ByteMap(ByteMap const &oth, unsigned int x, unsigned int y,
                 unsigned int w, unsigned int h): wid(w), hei(h),
                                                  store(wid*hei) {
  dat = store.data();
  for (unsigned int a = 0; a < hei; a++)
    memcpy(line(a), oth.cline(y + a) + x, wid);
}

ByteMap::ByteMap(ByteMap const &oth, int rot): wid(oth.wid), hei(oth.hei),
                                               store(wid*hei){
  if (hei != wid)
    throw("ByteMap: cannot rotate non-square map");
  dat = store.data();
  switch (rot & 3) {
  case 0:
    memcpy(dat, oth.dat, wid * hei);
    break;

  case 1:
    for (unsigned int y = 0; y < hei; y++)
      for (unsigned int x = 0; x < wid; x++)
        c(x, y) = oth.cc(y, wid - 1 - x);
    break;

  case 2:
    for (unsigned int y = 0; y < hei; y++)
      for (unsigned int x = 0; x < wid; x++)
        c(x, y) = oth.cc(wid - 1 - x, hei - 1 - y);
    break;

  case 3:
    for (unsigned int y = 0; y < hei; y++)
      for (unsigned int x = 0; x < wid; x++)
        c(x, y) = oth.cc(wid - 1 - y, x);
    break;
  }
}

void ByteMap::write(QString filename) const {
  QImage(dat, wid, hei, hei, QImage::Format_Grayscale8).save(filename);
}
