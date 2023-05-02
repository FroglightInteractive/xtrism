// ScreenPit.C

#include <math.h>

#include "ScreenPit.h"
#include "RGBRecolor.h"
#include "BBox.h"
#include <QPaintEvent>
#include <QPainter>

ScreenPit::ScreenPit(VisPit &vp, Rainbow const &sbg, QWidget *parent):
  QWidget(parent), sharedbg(sbg), vispit(vp) {
  wid = vp.cellsize() * vp.width();
  hei = vp.cellsize() * vp.height();
  bw = wid/100 + 1;
  resize(wid + 2*bw, hei + 2*bw);
}

ScreenPit::~ScreenPit() {
}

void ScreenPit::generate() {
  Rainbow clipped = sharedbg.cropped(geometry());
  int W = vispit.width();
  int dx = vispit.cellsize();
  BBox bb(bw, bw, bw+1, hei+bw);
  for (int x = 0; x < W * dx; x++) {
    double phase = (x + dx/2) * 3.141592 / dx;
    clipped.image().recolorRect(bb, .5 + .15 * cos(phase), 0);
    bb.shift(1, 0);
  }
  clipped.image().recolorRectEdges(BBox(QPoint(), size()), bw, -64);
  mybg = QPixmap::fromImage(clipped.create().toQImage());
  topleft = pos();
}

void ScreenPit::paintEvent(QPaintEvent *e) {
  BBox bbox(e->rect());
  QPainter p(this);
  if (pos()!=topleft || mybg.size()!=size())
    generate();
  p.drawPixmap(QPoint(0,0), mybg);
  
  int dx = vispit.cellsize();
  int dy = dx;
  int x0 = bw;
  int y0 = bw;
  for (int j = 0; j < vispit.height(); j++) {
    int y = y0 + dy * j;
    for (int i = 0; i < vispit.width(); i++) {
      int x = x0 + dx * i;
      BBox bb(x, y, x + dx, y + dy);
      if (bb.intersect(bbox)) 
        redrawcell(&p, x, y, vispit.cell(i, j));
    }
  }
}

void ScreenPit::poll() {
  for (int j = 0; j < vispit.height(); j++) {
    if (vispit.changedline(j)) {
      vispit.resetchanged();
      update();
      return;
    }
  }
}

void ScreenPit::redrawcell(QPainter *p, int x, int y, QPixmap const *tsp) const {
  if (tsp) 
    p->drawPixmap(QPoint(x, y), *tsp);
}
