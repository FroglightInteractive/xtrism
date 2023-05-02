// NiceSession.C

#include "NiceSession.h"
#include "../globals/Globals.h"
#include "../game/NiceGame.h"
#include "Options.h"
#include "../sound/Sounds.h"
#include "MainWindow.h"
#include <QFileInfo>
#include "brickcell.h"
#include "MarbleBG.h"
#include "Probability.h"
#include <QEventLoop>
#include <QPainter>
#include <QKeyEvent>

NiceSession::NiceSession(QString id, MainWindow *mw, QWidget *playbutton):
  QWidget(mw), bg_(mw->width(), mw->height()) {
  resize(mw->size());
  move(0, 0);
  QString fn = QString("%1/gamebg-%2-%3x%4.jpg").arg(cachedir()).arg(id)
    .arg(width()).arg(height());
  if (QFileInfo(fn).exists()) {
    bg_.load(fn);
  } else {
    BrickCell bc(width(), height());
    QRect pbr = playbutton->geometry();
    float l = pbr.left() * 1.0 / width();
    float t = pbr.top() * 1.0 / height();
    float r = pbr.right() * 1.0 / width();
    float b = pbr.bottom() *1.0 / height();
    marblebg(width(), height(), 0,
             l,t,r,b,
             bg_, 0, 0,
             &bc);
    bg_.save(fn);
  }
  bg = QPixmap::fromImage(bg_);
  mm = playbutton ? playbutton->parentWidget() : 0;
}

NiceSession::NiceSession(QString id,
                         Player const &p1, int bset,
                         MainWindow *mw, QWidget *playbutton):
  NiceSession(id, mw, playbutton) {
  g1 = new NiceGame(this, Sides::Side::Solo,
                    &p1, 0,
                    &options().metakeys(Sides::Side::Solo), 0,
                    sbd(), bs(), bs(),
                    bset);
  g2 = 0;
}

NiceSession::NiceSession(QString id,
                         Player const &p1, Player const &p2,
                         int bset,
                         MainWindow *mw, QWidget *playbutton):
  NiceSession(id, mw, playbutton) {
  g1 = new NiceGame(this, Sides::Side::Solo,
                    &p1, &p2, 
                    &options().metakeys(Sides::Side::Left),
                    &options().metakeys(Sides::Side::Right),
                    sbd(), bs(), bs2(),
                    bset);
  g2 = 0;
}

NiceSession::NiceSession(QString id,
                         Player const &p1, Player const &p2,
                         int bset1, int bset2,
                         MainWindow *mw, QWidget *playbutton):
  NiceSession(id, mw, playbutton) {
  g1 = new NiceGame(this, Sides::Side::Left,
                    &p1, 0, 
                    &options().metakeys(Sides::Side::Left), 0,
                    sbd(), bs(), bs(),
                    bset1);
  g2 = new NiceGame(this, Sides::Side::Right,
                    &p2, 0,
                    &options().metakeys(Sides::Side::Right), 0,
                    sbd(), bs(), bs(),
                    bset2);
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
  if (mm)
    mm->show();
}

void NiceSession::paintEvent(QPaintEvent *) {
  QPainter p(this);
  p.drawPixmap(0, 0, bg);
}


RGBMap const &NiceSession::background() const {
  return bg_;
}

void NiceSession::keyPressEvent(QKeyEvent *e) {
  // if (e->isAutoRepeat())
  //  return;
  g1->key(e->key(), true);
  if (g2)
    g2->key(e->key(), true);
}

void NiceSession::keyReleaseEvent(QKeyEvent *e) {
  // if (e->isAutoRepeat())
  //  return;
  g1->key(e->key(), false);
  if (g2)
    g2->key(e->key(), false);
}
