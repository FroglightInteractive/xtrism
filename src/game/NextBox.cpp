// NextBox.C

#include "NextBox.h"
#include "../basics/Infty.h"
#include "../bricks/data.h"
#include "../bricks/bsprites.h"
#include "../pics/Recolour.h"
#include <QPainter>

NextBox::NextBox(SBrickData const &sbd0, BrickSprites const &bs0,
                 RGBMap const &sbg, QWidget *parent):
  QWidget(parent),
  // GBox(p, QSize((sbd0.maxlines() + 1) * bs0.size())),
  sbd(sbd0), bs(bs0), sharedbg(sbg),
  bno(-1), rot(0) {
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
  RGBMap clipped(sharedbg, pos().x(), pos().y(), width(), height());
  int bw = 1 + width() / 50;
  recolour_circle(&clipped, QPoint(width()/2, height()/2),
                  width() / 2 - bw - 1, .5, 0,
                  width() / 2 - 1, -64);
  mybg = QPixmap::fromImage(clipped);
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
    x0 -= int(size * (xave / bd.cells() - 1.5));
    y0 -= int(size * (yave / bd.cells() - 1.5));
    for (unsigned int i = 0; i < bd.cells(); i++)
      p.drawPixmap(x0 + bd.x(i)*size, y0 + bd.y(i)*size, bs.cell(bno, rot, i));
  }
}
