// RGBMap.C

#include <stdlib.h>
#include "RGBMap.h"
#include "../basics/dbx.h"

RGBMap::RGBMap(unsigned int w, unsigned int h):
  QImage(w, h, QImage::Format_RGBX8888),
  wid(w), hei(h) {
  dat = (RGB*)bits();
  memset(dat, 0, sizeof(RGB) * wid * hei);
}

void RGBMap::load(QString filename) {
  QImage::load(filename);
  if (format()!=QImage::Format_RGBX8888)
    convertTo(QImage::Format_RGBX8888);
  wid = width();
  hei = height();
  dat = (RGB*)bits();
}  

RGBMap::RGBMap(QString filename): QImage(filename) {
  if (format() != QImage::Format_RGBX8888)
    convertTo(QImage::Format_RGBX8888);
  wid = width();
  hei = height();
  dat = (RGB*)bits();
}

RGBMap::~RGBMap() {
}

RGBMap::RGBMap(RGBMap const &oth, unsigned int x, unsigned int y,
               unsigned int w, unsigned int h): RGBMap(w, h) {
  for (unsigned int a = 0; a < hei; a++)
    memcpy(line(a), oth.cline(y + a) + x, wid * sizeof(RGB));
}

RGBMap::RGBMap(RGBMap const &oth, int rot): RGBMap(oth.wid, oth.hei) {
  if (hei != wid)
    throw "RGBMap: cannot rotate non-square map";
  switch (rot & 3) {
  case 0:
    memcpy(dat, oth.dat, sizeof(RGB) * wid * hei);
    break;
  case 1:
    for (unsigned int y = 0; y < hei; y++)
      for (unsigned int x = 0; x < wid; x++)
        c(x, y) = oth.cc(y, wid - 1 - x);
    break;
  case 2:
    for (unsigned int y = 0; y < hei; y++)
      for (unsigned int x = 0; x < wid; x++)
        c(x, y) = oth.cc(hei - 1 - y, wid - 1 - x);
    break;
  case 3:
    for (unsigned int y = 0; y < hei; y++)
      for (unsigned int x = 0; x < wid; x++)
        c(x, y) = oth.cc(wid - 1 - y, x);
    break;
  }
}
