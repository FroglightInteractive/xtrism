// NewPlayer.cpp

#include "NewPlayer.h"
#include "TranspLabel.h"
#include <QKeyEvent>
#include <QEventLoop>

NewPlayer::NewPlayer(QWidget *parent): MarbledButton(parent) {
  resize(parent->width()/3, parent->height()/6);
  
  auto *lbl = new TranspLabel("Welcome new player!", QColor(255,255,0),
                              Qt::AlignCenter, this);
  lbl->setGeometry(QRect(0,0,width(),height()*.3));
  lbl = new TranspLabel("Please enter your name:", QColor(255,255,255),
                              Qt::AlignCenter, this);
  lbl->setGeometry(QRect(0,height()*.3,width(),height()*.3));
  editor_ = new TranspLabel("|", QColor(255,255,0),
                            Qt::AlignCenter, this);
  editor_->setGeometry(QRect(0,height()*.6,width(),height()*.4));
  connect(editor_, &TranspLabel::keyPressed,
          this, [this](int scan, QString txt, int qkey) {
            keyPress(txt, qkey);
          });
  pos_ = 0;
}

NewPlayer::~NewPlayer() {
}

QString NewPlayer::name() const {
  return name_;
}

bool NewPlayer::exec() {
  QEventLoop el;
  bool res = false;
  show();
  grabMouse();
  grabKeyboard();
  connect(this, &NewPlayer::cancel,
          this, [&res, &el]() {
            res = false;
            el.quit();
          });
  connect(this, &NewPlayer::accept,
          this, [&res, &el]() {
            res = true;
            el.quit();
          });
  el.exec();
  releaseKeyboard();
  releaseMouse();
  if (!res)
    name_ = "";
  qDebug() << "new player" << name_;
  return name_ != "";
}

void NewPlayer::keyPressEvent(QKeyEvent *e) {
  keyPress(e->text(), e->key());
  e->accept();
}

void NewPlayer::keyPress(QString txt, int qkey) {
  qDebug() << "keypress" << txt << qkey;
  switch (qkey) {
  case Qt::Key_Backspace:
    qDebug() << "backspace";
    if (pos_ > 0) {
      --pos_;
      name_ = name_.left(pos_) + name_.mid(pos_+1);
      rebuild();
    }
    break;
  case Qt::Key_Delete:
    if (pos_ < name_.size()) {
      name_ = name_.left(pos_) + name_.mid(pos_+1);
      rebuild();
    }
    break;
  case Qt::Key_Left:
    qDebug() << "left";
    if (pos_ > 0) {
      --pos_;
      rebuild();
    }
    break;
  case Qt::Key_Right:
    if (pos_ < name_.size()) {
      ++pos_;
      rebuild();
    }
    break;
  case Qt::Key_Home:
    pos_ = 0;
    rebuild();
    break;
  case Qt::Key_End:
    pos_ = name_.size();
    rebuild();
    break;
  case Qt::Key_Enter: case Qt::Key_Return:
    emit accept();
    break;
  case Qt::Key_Escape:
    emit cancel();
    break;
  default:
    if (txt.size()) {
      name_ = name_.left(pos_) + txt + name_.mid(pos_);
      pos_ += txt.size();
      rebuild();
    }
  }
}

void NewPlayer::rebuild() {
  editor_->setText(name_.left(pos_) + "|" + name_.mid(pos_));
}

