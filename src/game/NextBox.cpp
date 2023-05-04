// NextBox.C

#include "NextBox.h"
#include "BrickData.h"
#include "BrickSprites.h"
#include <QPainter>
#include "RGBRecolor.h"
#include <QDebug>

NextBox::NextBox(SBrickData const &sbd0, BrickSprites const &bs0,
                 Rainbow const &sbg, QWidget *parent):
  QWidget(parent),
  // GBox(p, QSize((sbd0.maxlines() + 1) * bs0.size())),
  sbd(sbd0), bs(bs0), sharedbg(sbg),
  bno(-1), rot(0) {
  int w = (sbd0.maxlines() + 1) * bs0.size();
  resize(w, w);
}

NextBox::~NextBox() {
}

void NextBox::setbrick(int bno0, int rot0) {
  //  if (logger)
  //    logger->newnext(myid, bno0, rot0);
  bno = bno0;
  rot = rot0;
  update();
}

void NextBox::generate() {
  Rainbow clipped = sharedbg.cropped(geometry());
  int bw = 1 + width() / 50;
  clipped.image().recolorCircle(QPoint(width()/2, height()/2),
                  width() / 2 - bw - 1, .5, 0,
                  width() / 2 - 1, -64);
  mybg = QPixmap::fromImage(clipped.create().toQImage());
  topleft = pos();
}

void NextBox::paintEvent(QPaintEvent *) {
  QPainter p(this);
  if (pos()!=topleft || mybg.size()!=size())
    generate();
  p.drawPixmap(QPoint(0,0), mybg);
  if (bno >= 0) {
    BrickData const &bd = sbd.brick(bno, rot);
    int size = bs.size();
    int x0 = width()/2;
    int y0 = height()/2;
    float xave = 0;
    float yave = 0;
    for (unsigned int i = 0; i < bd.cells(); i++) {
      xave += bd.x(i);
      yave += bd.y(i);
    }
    qDebug() << xave/4 << yave/4;
    x0 -= int(size * (xave / bd.cells() + .5));
    y0 -= int(size * (yave / bd.cells() + .5));
    for (unsigned int i = 0; i < bd.cells(); i++)
      p.drawPixmap(x0 + bd.x(i)*size, y0 + bd.y(i)*size, bs.cell(bno, rot, i));
  }
}
