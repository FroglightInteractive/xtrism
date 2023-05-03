// NiceSession.C

#include "NiceSession.h"
#include "NiceGame.h"
#include "Options.h"
#include "Sounds.h"
#include "MainWindow.h"
#include <QFileInfo>
#include "BrickCell.h"
#include "Probability.h"
#include <QEventLoop>
#include <QPainter>
#include <QKeyEvent>
#include "Paths.h"
#include "BrickData.h"
#include "SPlayer.h"

NiceSession::NiceSession(QString id, MainWindow *mw, QWidget *playbutton):
  QWidget(mw) {
  resize(mw->size());
  move(0, 0);
  QString fn0 = QString("%1/gamebg0-%2-%3x%4.jpg")
    .arg(Paths::cachedir()).arg(id)
    .arg(width()).arg(height());
  QString fn = QString("%1/gamebg-%2-%3x%4.jpg")
    .arg(Paths::cachedir()).arg(id)
    .arg(width()).arg(height());
  QRect src = playbutton->geometry();
  QSize base = mw->size();
  if (QFileInfo(fn0).exists()) {
    bg_ = Rainbow(GrayImage::fromFile(fn0), src, base);
  } else {
    bg_ = Rainbow(size(), src, base);
    bg_.image().save(fn0);
  }
  if (QFileInfo(fn).exists()) {
    bg = QPixmap(fn);
  } else {
    QImage img = bg_.create().toQImage();
    img.save(fn);
    bg = QPixmap::fromImage(img);
  }
  mm = playbutton ? playbutton->parentWidget() : 0;
}

NiceSession::NiceSession(QString id,
                         Player const &p1, int bset,
                         MainWindow *mw, QWidget *playbutton):
  NiceSession(id, mw, playbutton) {
  g1 = new NiceGame(this, Sides::Side::Solo,
                    &p1, 0,
                    &Options::instance().metakeys(Sides::Side::Solo), 0,
                    SBrickData::instance(), mw->brickSprites(), 0,
                    bset, mw->xworld());
  g2 = 0;
}

NiceSession::NiceSession(QString id,
                         Player const &p1, Player const &p2,
                         int bset,
                         MainWindow *mw, QWidget *playbutton):
  NiceSession(id, mw, playbutton) {
  g1 = new NiceGame(this, Sides::Side::Solo,
                    &p1, &p2, 
                    &Options::instance().metakeys(Sides::Side::Left),
                    &Options::instance().metakeys(Sides::Side::Right),
                    SBrickData::instance(),
                    mw->brickSprites(), mw->brickSprites(true),
                    bset, mw->xworld());
  g2 = 0;
}

NiceSession::NiceSession(QString id,
                         Player const &p1, Player const &p2,
                         int bset1, int bset2,
                         MainWindow *mw, QWidget *playbutton):
  NiceSession(id, mw, playbutton) {
  g1 = new NiceGame(this, Sides::Side::Left,
                    &p1, 0, 
                    &Options::instance().metakeys(Sides::Side::Left), 0,
                    SBrickData::instance(), mw->brickSprites(), 0,
                    bset1, mw->xworld());
  g2 = new NiceGame(this, Sides::Side::Right,
                    &p2, 0,
                    &Options::instance().metakeys(Sides::Side::Right), 0,
                    SBrickData::instance(), mw->brickSprites(), 0,
                    bset2, mw->xworld());
}

NiceSession::~NiceSession() {
  delete g1;
  delete g2;
}

void NiceSession::exec() {
  if (mm)
    mm->hide();
  show();
  setFocus();
  QEventLoop *el = new QEventLoop(this);
  connect(g1, &NiceGame::quit,
          el, [this, el]() {
            if (!g2 || !g2->isplaying())
              el->quit();
          });
  if (g2)
    connect(g2, &NiceGame::quit,
            el, [this, el]() {
              if (!g1->isplaying())
                el->quit();
            });
  g1->start();
  if (g2)
    g2->start();
  el->exec();
  delete el;
  hide();
  if (mm) {
    mm->show();
    mm->setFocus();
  }
}

void NiceSession::paintEvent(QPaintEvent *) {
  QPainter p(this);
  p.drawPixmap(0, 0, bg);
}


Rainbow const &NiceSession::background() const {
  return bg_;
}

void NiceSession::keyPressEvent(QKeyEvent *e) {
  // if (e->isAutoRepeat())
  //  return;
  g1->key(e->nativeScanCode(), true);
  if (g2)
    g2->key(e->nativeScanCode(), true);
  switch (e->key()) {
  case Qt::Key_Q:
    SPlayer::instance()->toggleSounds();
    break;
  }
}

void NiceSession::keyReleaseEvent(QKeyEvent *e) {
  // if (e->isAutoRepeat())
  //  return;
  g1->key(e->nativeScanCode(), false);
  if (g2)
    g2->key(e->nativeScanCode(), false);
}
