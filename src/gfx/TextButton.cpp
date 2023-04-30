// TextButton.cpp

#include "TextButton.h"

TextButton::TextButton(QString cachedir, QWidget *parent):
  MarbledButton(cachedir, parent) {
  hasfnt = false;
}


TextButton::~TextButton() {
}

void TextButton::paintEvent(QPaintEvent *evt) {
  MarbledButton::paintEvent(evt);
  QPainter *p(this);
  p.setPen(QColor(255,255,255));
  p.drawText(QRect(QPoint(0,0), size()), Qt::AlignCenter, txt);
}
