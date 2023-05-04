// Rainbow.cpp

#include "Rainbow.h"
#include "BrickCell.h"
#include <QDebug>

QRectF getRect(QRect r, QSize s) {
  float l = r.left()*1./s.width();
  float w = r.width()*1./s.width();
  float t = r.top()*1./s.height();
  float h = r.height()*1./s.height();
  return QRectF(l, t, w, h);
}
  
QRectF subRect(QRectF rr, QRect r, QSize s) {
  float l = r.left()*1./s.width();
  float w = r.width()*1./s.width();
  float t = r.top()*1./s.height();
  float h = r.height()*1./s.height();
  return QRectF(rr.left() + l*rr.width(),
                rr.top() + t*rr.height(),
                w*rr.width(),
                h*rr.height());
}

Rainbow::Rainbow(): gry(0,0) {
}

Rainbow::Rainbow(GrayImage img, QRect source, QSize base): gry(img) {
  rr = getRect(source, base);
}

Rainbow::Rainbow(QSize size, QRect source, QSize base): gry(0,0) {
  qDebug() << "rainbow" << size << source << base;
  rr = getRect(source, base);
  gry = BrickCell(size.width(), size.height());
}
  
RGBImage Rainbow::create() const {
  qDebug() << "rainbow create" << gry.size() << rr;
  return RGBImage::colorized(gry, rr);
}

Rainbow Rainbow::cropped(QRect source) const {
  Rainbow out;
  out.rr = subRect(rr, source, gry.size());
  out.gry = gry.cropped(source.x(), source.y(),
                        source.width(), source.height());
  return out;
}

