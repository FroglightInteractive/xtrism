// TextButton.cpp

#include "TextButton.h"
#include <QPainter>

TextButton::TextButton(QWidget *parent):
  MarbledButton(parent) {
}


TextButton::~TextButton() {
}

void TextButton::setText(QString txt1) {
  txt = txt1;
  update();
}

void TextButton::paintEvent(QPaintEvent *evt) {
  MarbledButton::paintEvent(evt);
  QPainter p(this);
  p.setPen(QColor(255,255,255));
  p.drawText(QRect(QPoint(0,0), size()), Qt::AlignCenter, txt);
}
