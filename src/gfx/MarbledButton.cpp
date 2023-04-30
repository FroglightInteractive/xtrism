// MarbledButton.cpp

#include "MarbledButton.h"

MarbledButton::MarbledButton(QString cachedir, QWidget *parent):
  QWidget(parent), cachedir(cachedir) {
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
      .arg(psize().width()).arg(psize.height());
    QString fn = QString("%1/%2.jpg").arg(cachedir).arg(id);
    if (QFileInfo(fn).exists()) {
      bg.load(fn);
    } else {
      generate();
      bg.save(fn);
    }
  }
  p.drawPixmap(bg);
}

void MarbledButton::generate() {
  QWidget *top = window();
  QSizeF psize = top() ? top->size() : size();
  float left = top ? pos().x() / psize.width() : 0;
  float right = top ? (pos().x()+width()) / psize.width() : 1;
  float top = top ? pos().y() / psize.height() : 0; 
  float bottom = top ? (pos().y()+height()) / psize.height() : 1;
  RGBMap img(width(), height());
  marblebg(width(), height(), 2,
           left, top, right, bottom,
           img, 0, 0);
  bg = QPixmap::fromImage(img);
}
