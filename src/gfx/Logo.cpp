// Logo.cpp

#include "Logo.h"

#define NAME " T R I S M "

#include <QFont>
#include <QFontMetrics>
#include "RGBImage.h"
#include "BrickCell.h"
#include <QPainter>
#include <QDebug>
#include <QDateTime>

bool _sizeInside(QSize a, QSize b, int margin) {
  return a.width() + margin < b.width()
    && a.height() + margin < b.height();
}

QImage logo(QRect dest, QSize mwsize, int depth) {
  qDebug() << QDateTime::currentDateTime() << "logo";
  QSize size = dest.size();
  QRectF rainbow(QPointF(dest.left()*1./mwsize.width(),
                         dest.top()*1./mwsize.height()),
                 QSizeF(dest.width()*1./mwsize.width(),
                        dest.height()*1./mwsize.height()));
  QFont f("Benguiat Bk BT");
  int border = size.width() / 1000 + 1;
  int margin = 2*border + 16;
  int fs = 0;
  for (int bit=10; bit>=0; --bit) {
    f.setPixelSize(fs + (1<<bit));
    if (_sizeInside(QFontMetrics(f).boundingRect(NAME).size(),
                    size, margin))
      fs += 1<<bit;
  }
  f.setPixelSize(fs);
  size = QFontMetrics(f).boundingRect(NAME).size();
  int w = size.width();
  int h =  size.height();
  qDebug() << QDateTime::currentDateTime()
         << "pixelsize" << fs-1 << border << NAME << size;

  QImage bulk(size, QImage::Format_Grayscale8);
  bulk.fill(0);
  QPainter p;
  p.begin(&bulk);
  p.setFont(f);
  p.setPen(QColor(255,255,255));
  p.drawText(QRect(QPoint(0, 0), size), Qt::AlignCenter, NAME);
  p.end();

  QImage dark = bulk;
  p.begin(&dark);
  p.setFont(f);
  p.setPen(QColor(0,0,0));
  for (int dy=-2*border; dy<=-border; dy++)
    for (int dx=-2*border; dx<=-border; dx++)
      //if (dx+dy<=-border)
        p.drawText(QRect(QPoint(dx, dy), size), Qt::AlignCenter, NAME);
  p.end();
  QImage light = bulk;
  p.begin(&light);
  p.setFont(f);
  p.setPen(QColor(0,0,0));
  for (int dy=border; dy<=2*border; dy++)
    for (int dx=border; dx<=2*border; dx++)
      //if (dx+dy>=border)
        p.drawText(QRect(QPoint(dx, dy), size), Qt::AlignCenter, NAME);
  p.end();
  qDebug() << QDateTime::currentDateTime() << "drawn";
  uint8_t *bulkp = bulk.bits();
  int bulkl = bulk.bytesPerLine();
  uint8_t *darkp = dark.bits();
  int darkl = dark.bytesPerLine();
  uint8_t *lightp = light.bits();
  int lightl = light.bytesPerLine();
  BrickCell bc(w, h);
  for (int y=0; y<h; y++)
    for (int x=0; x<w; x++)
      if (darkp[y*darkl+x] && !lightp[y*lightl+x])
        bc.alter(x, y, -depth);
  for (int y=0; y<h; y++)
    for (int x=0; x<w; x++)
      if (lightp[y*lightl+x] && !darkp[y*darkl+x])
        bc.alter(x, y, depth);
  qDebug() << QDateTime::currentDateTime() << "copied";
  QImage img(RGBImage::colorized(bc, rainbow).toQImage()
             .convertToFormat(QImage::Format_ARGB32));
  qDebug() << QDateTime::currentDateTime() << "colorized";
  uint32_t *bits = (uint32_t*)(img.bits());
  int wpl = img.bytesPerLine()/4;
  for (int y=0; y<h; y++)
    for (int x=0; x<w; x++)
      if (!bulkp[y*bulkl+x] && !lightp[y*lightl+x] && !darkp[y*darkl+x])
        bits[wpl*y + x] = 0;
  qDebug() << QDateTime::currentDateTime() << "etched";
  return img;
}
  
