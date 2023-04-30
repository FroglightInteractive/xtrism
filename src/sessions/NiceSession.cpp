// NiceSession.C

#include "NiceSession.h"
#include "../globals/Globals.h"
#include "../game/NiceGame.h"

#include "../sound/Sounds.h"
#include "MainWindow.h"
#include <QFileInfo>

NiceSession::NiceSession(QString id, MainWindow *mw, QWidget *playbutton):
  QWidget(mw), bg_(mw->width(), mw->height()) {
  resize(mw->size());
  move(0, 0);
  QString fn = QString("%1/gamebg-%2-%3x%4.jpg").arg(mw->cachedir).arg(id)
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
             bc);
    rgb.save(fn);
  }
  bg = QPixmap::fromImage(bg_);
}

NiceSession::NiceSession(QString id,
                         Player const &p1, int bset,
                         MainWindow *mw, QWidget *playbutton):
  NiceSession(id, mw, playbutton) {
  dbx(1, "NiceSession: Solo");
  g1 = new NiceGame(this, 0,
                    &p1, 0, globalopts(),
                    sbd(), bs(), bs(),
                    bset, probabilities(bset));
  g2 = 0;
}

NiceSession::NiceSession(QString id,
                         Player const &p1, Player const &p2,
                         int bset,
                         MainWindow *mw, QWidget *playbutton):
  NiceSession(id, mw, playbutton) {
  dbx(1, "NiceSession: Team");
  g1 = new NiceGame(this, 0,
                    &p1, &p2, globalopts(),
                    sbd(), bs(), bs2(),
                    bset);
  g2 = 0;
}

NiceSession::NiceSession(QString id,
                         Player const &p1, Player const &p2,
                         int bset1, int bset2,
                         MainWindow *mw, QWidget *playbutton):
  NiceSession(id, mw, playbutton) {
  dbx(1, "NiceSession: Apart");
  g1 = new NiceGame(this, -1,
                    &p1, 0, globalopts(),
                    sbd(), bs(), bs(),
                    bset1);
  g2 = new NiceGame(this, 1,
                    &p2, 0, globalopts(),
                    sbd(), bs(), bs(),
                    bset2);
}

NiceSession::~NiceSession() {
  delete g1;
  delete g2;
}

void NiceSession::exec() {
  takeFocus();
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
  return;
}

void NiceSession::paintEvent(QPaintEvent *) {
  QPainter p(this);
  p.drawPixmap(bg);
}

void NiceSession::keyPressEvent(QKeyEvent *e) {
}

void NiceSession::keyReleaseEvent(QKeyEvent *e) {
}

RGBMap const &NiceSession::background() {
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
