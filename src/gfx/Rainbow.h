// Rainbow.h

#ifndef RAINBOW_H

#define RAINBOW_H

#include "RGBImage.h"
#include "GrayImage.h"

class Rainbow {
public:
  Rainbow();
  Rainbow(GrayImage precalced, QRect source, QSize base);
  Rainbow(QSize size, QRect source, QSize base);
  RGBImage create() const;
  Rainbow cropped(QRect source) const;
  GrayImage &image() { return gry; } // so it can be recolored
private:
  GrayImage gry;
  QRectF rr;
};

#endif
