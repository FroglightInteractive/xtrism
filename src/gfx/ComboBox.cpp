// ComboBox.cpp

#include "ComboBox.h"
#include <QMouseEvent>
#include "MenuBox.h"

ComboBox::ComboBox(QWidget *parent): TextButton(parent) {
}

ComboBox::~ComboBox() {
}

bool ComboBox::selectItem(QString s) {
  int id1 = items_.indexOf(s);
  if (id1>=0) {
    idx_ = id1;
    setText(s);
    return true;
  } else {
    return false;
  }
}

void ComboBox::setItems(QStringList its) {
  items_ = its;
  if (items_.size()) {
    idx_ = 0;
    setText(items_[0]);
    emit itemChanged(items_[0]);
  } else {
    idx_ = - 1;
    setText("");
    emit itemChanged("");
  }
}

void ComboBox::mousePressEvent(QMouseEvent *e) {
  if (e->button() != Qt::LeftButton)
    return;
  if (items_.size() <= 0)
    return;

  MenuBox mb(parentWidget());
  int h0 = height()*6;
  int h = mb.heightForLineCount(items_.size());
  if (h > h0)
    h = h0;
  QRect r = geometry();
  mb.setGeometry(QRect(r.left(), r.bottom() + r.height()/10, r.width(), h));
  mb.setItems(items_);
  mb.selectItem(items_[idx_]);
  QString res = mb.exec();
  if (selectItem(res))
    emit itemChanged(res);
}
