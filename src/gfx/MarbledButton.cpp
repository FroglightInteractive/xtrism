// MarbledButton.cpp

#include "MarbledButton.h"
#include <QPainter>
#include <QFileInfo>
#include "Globals.h"
#include "RGBImage.h"

MarbledButton::MarbledButton(QWidget *parent):
  QWidget(parent) {
}

MarbledButton::~MarbledButton() {
}

void MarbledButton::paintEvent(QPaintEvent *) {
  QPainter p(this);
  QWidget *top = window();
  QSize psize = top ? top->size() : QSize(1,1);
  if (bg.size() != size() || pos()!=topleft) {
    QString id = QString("marblebg-%1x%2+%3+%4-%5x%6")
      .arg(width()).arg(height()).arg(pos().x()).arg(pos().y())
      .arg(psize.width()).arg(psize.height());
    QString fn = QString("%1/%2.jpg").arg(cachedir()).arg(id);
    if (QFileInfo(fn).exists()) {
      bg.load(fn);
    } else {
      generate();
      bg.save(fn);
    }
  }
  p.drawPixmap(0, 0, bg);
}

void MarbledButton::generate() {
  topleft = pos();
  QWidget *w = window();
  QSizeF psize = w ? w->size() : size();
  float left = w ? pos().x() / psize.width() : 0;
  float right = w ? (pos().x()+width()) / psize.width() : 1;
  float top = w ? pos().y() / psize.height() : 0; 
  float bottom = w ? (pos().y()+height()) / psize.height() : 1;
  RGBImage img(RGBImage::colorized(size(), 2,
                                   QRectF(left, top, right-left, bottom-top)));
               bg = QPixmap::fromImage(img.toQImage());
}
