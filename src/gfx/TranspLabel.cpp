// TranspLabel.cpp

#include "TranspLabel.h"
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>

TranspLabel::TranspLabel(QWidget *parent): QWidget(parent) {
  text_ = "";
  color_ = QColor(255, 255, 255);
  alignment_ = Qt::AlignCenter;
}

TranspLabel::TranspLabel(QString s, QColor c, Qt::Alignment a, QWidget *parent):
  QWidget(parent),
  text_(s), color_(c), alignment_(a) {
}

TranspLabel::~TranspLabel() {
}

void TranspLabel::setText(QString s) {
  text_ = s;
  update();
}

void TranspLabel::setColor(QColor c) {
  color_ = c;
  update();
}

void TranspLabel::setAlignment(Qt::Alignment a) {
  alignment_ = a;
  update();
}

void TranspLabel::paintEvent(QPaintEvent *) {
  QPainter p(this);
  QPen pen(p.pen());
  if (hasFocus())
    pen.setColor(QColor(255,255,0));
  else
    pen.setColor(color_);
  p.setPen(pen);
  p.drawText(QRect(0, 0, width(), height()), alignment_, text_);
}

void TranspLabel::mousePressEvent(QMouseEvent *e) {
  if (e->button()!=Qt::LeftButton)
    return;
  setFocus();
  update();
  emit mousePressed();
}

void TranspLabel::enterEvent(QEnterEvent *e) {
  setFocus();
  update();
}

void TranspLabel::keyPressEvent(QKeyEvent *e) {
  emit keyPressed(e->nativeScanCode(), e->text(), e->key());
}

void TranspLabel::focusInEvent(QFocusEvent *) {
  update();
}

void TranspLabel::focusOutEvent(QFocusEvent *) {
  update();
}
