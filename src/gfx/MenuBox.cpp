// MenuBox.cpp

#include "MenuBox.h"
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QEventLoop>
#include <QTimer>

MenuBox::MenuBox(QWidget *parent): MarbledButton(parent) {
  idx_ = -1;
  onarrow_ = 0;
  top_ = 0;
  setMouseTracking(true);
  scrollTimer_ = new QTimer(this);
  scrollTimer_->setInterval(std::chrono::milliseconds(200));
  connect(scrollTimer_, &QTimer::timeout,
          this, [this]() { scroll(onarrow_, false); });
}

MenuBox::~MenuBox() {
}

void MenuBox::setItems(QStringList const &lst) {
  items_ = lst;
  update();
}

bool MenuBox::selectItem(QString s) {
  int idx1 = items_.indexOf(s);
  if (idx1>=0) {
    idx_ = idx1;
    update();
    return true;
  } else {
    return false;
  }
}

int MenuBox::heightForLineCount(int n) const {
  return topMargin() + bottomMargin() + lineHeight()*n;
}

void MenuBox::paintEvent(QPaintEvent *e) {
  MarbledButton::paintEvent(e);
  QPainter p(this);
  int y = topMargin();
  int dy = lineHeight();
  int y1 = height() - bottomMargin() - dy;
  if (needsArrows()) {
    drawUpArrow(&p);
    drawDownArrow(&p);
    y += arrowHeight();
    y1 -= arrowHeight();
  }
  for (int k=top_; k<items_.size() && y<=y1; k++, y+=dy) 
    drawItem(&p, k, y);
}

void MenuBox::drawUpArrow(QPainter *p) {
  QPen pen(p->pen());
  if (onarrow_ < 0)
    pen.setColor("#ffff00");
  else
    pen.setColor("#ffffff");
  p->setPen(pen);
  int y0 = topMargin();
  int dy = y0;
  for (int k=0; k<2; k++)
    p->drawText(QRect(0, int(y0 + k*dy),
                      width(), arrowHeight()),
                Qt::AlignCenter, "^");
}

void MenuBox::drawDownArrow(QPainter *p) {
  p->save();
  p->rotate(180);
  QPen pen(p->pen());
  if (onarrow_ > 0)
    pen.setColor("#ffff00");
  else
    pen.setColor("#ffffff");
  p->setPen(pen);
  int y0 = bottomMargin();
  int dy = y0;
  for (int k=0; k<2; k++)
    p->drawText(QRect(-width(), -height() + int(y0 + k*dy),
                      width(), arrowHeight()),
                Qt::AlignCenter, "^");
  p->restore();
}

void MenuBox::drawItem(QPainter *p, int idx, int y) {
  if (idx<0 || idx>=items_.size())
    return;
  QPen pen(p->pen());
  if (idx == idx_)
    pen.setColor("#ffff00");
  else
    pen.setColor("#ffffff");
  p->setPen(pen);
  p->drawText(QRect(0, y, width(), lineHeight()),
                Qt::AlignCenter, items_[idx]);
}

int MenuBox::itemAt(int y) const {
  int y0 = topMargin();
  if (needsArrows()) {
    if (y < y0 + arrowHeight())
      return -1;
    else if (y >= height() - arrowHeight() - bottomMargin())
      return items_.size();
    y0 += arrowHeight();
  }
  int k = (y - y0) / lineHeight() + top_;
  return k;
}
      
  
void MenuBox::keyPressEvent(QKeyEvent *e) {
  if (e->key()==Qt::Key_Escape)
    emit cancel();
}

void MenuBox::mousePressEvent(QMouseEvent *e) {
  if (e->pos().x() < 0 || e->pos().y() < 0
      || e->pos().x() >= width() || e->pos().y() >= height()) {
    emit cancel();
    return;
  }
  int k = itemAt(e->pos().y());
  if (k>=0 && k<items_.size())
    emit itemClicked(items_[k]);
}

void MenuBox::mouseReleaseEvent(QMouseEvent *e) {
  if (e->pos().x() < 0 || e->pos().y() < 0
      || e->pos().x() >= width() || e->pos().y() >= height())
    return;
  int k = itemAt(e->pos().y());
  if (k>=0 && k<items_.size())
    emit itemClicked(items_[k]);
}

void MenuBox::mouseMoveEvent(QMouseEvent *e) {
  int oldidx = idx_;
  int oldarrow = onarrow_;
  int k = itemAt(e->pos().y());
  idx_ = -1;
  onarrow_ = 0;
  if (k<0) {
    if (needsArrows())
      onarrow_ = -1;
  } else if (k>=items_.size()) {
    if (needsArrows())
      onarrow_ = 1;
  } else {
    idx_ = k;
  }
  if (onarrow_ && !oldarrow) {
    scroll(onarrow_, false);
    scrollTimer_->start();
  } else if (oldarrow && !onarrow_) {
    scrollTimer_->stop();
  }
  
  if (idx_ != oldidx || onarrow_ != oldarrow)
    update();
}
    
QString MenuBox::exec() {
  QString res;
  QEventLoop el;
  connect(this, &MenuBox::cancel, &el, &QEventLoop::quit);
  connect(this, &MenuBox::itemClicked, this,
          [&res, &el](QString s) { res = s; el.quit(); });
  show();
  grabMouse();
  grabKeyboard();
  el.exec();
  releaseKeyboard();
  releaseMouse();
  return res;
}

bool MenuBox::needsArrows() const {
  return height() < heightForLineCount(items_.size());
}

int MenuBox::lineHeight() const {
  return int(font().pixelSize()*1.2);
}

int MenuBox::topMargin() const {
  return int(.2*font().pixelSize());
}

int MenuBox::bottomMargin() const {
    return int(.2*font().pixelSize());
}

int MenuBox::arrowHeight() const {
  return int(.8*font().pixelSize());
}

void MenuBox::scroll(int dy, bool page) {
  int N = (height() - topMargin() - bottomMargin() - 2*arrowHeight())
    / lineHeight();
  top_ += dy * (page ? (N-1) : 1);
  if (top_ < 0)
    top_ = 0;
  if (top_ > items_.size() - N)
    top_ = items_.size() - N;
  qDebug() << "click" << N << top_;
  update();
}
