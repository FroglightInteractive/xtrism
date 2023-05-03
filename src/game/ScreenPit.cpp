// ScreenPit.C

#include <math.h>

#include "ScreenPit.h"
#include "RGBRecolor.h"
#include "BBox.h"
#include <QPaintEvent>
#include <QPainter>
#include <QTime>
#include "XWorld.h"

ScreenPit::ScreenPit(VisPit &vp, Rainbow const &sbg, QWidget *parent,
                     BrickSprites const &bs0, BrickSprites const &bs1):
  QWidget(parent), sharedbg(sbg), vispit(vp), bs0(bs0), bs1(bs1) {
  wid = vp.cellsize() * vp.width();
  hei = vp.cellsize() * vp.height();
  bw = wid/100 + 1;
  resize(wid + 2*bw, hei + 2*bw);
  xworld = 0;
}

ScreenPit::~ScreenPit() {
  delete xworld;
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
  RGBImage bg1 = clipped.create();
  mybg = QPixmap::fromImage(bg1.toQImage());
  topleft = pos();

  xworld = new XWorld(qApp, this);
  if (!xworld->active()) {
    qDebug() << "xworld not active";
    delete xworld;
    xworld = 0;
    return;
  }
  
  bgpixmap = xworld->storePixmap(bg1);
  for (int bno=0; bno<34; bno++) {
    for (int rot=0; rot<4; rot++) {
      for (int cel=0; cel<4; cel++) {
        brickpixmaps[&bs0.cell(bno, rot, cel)] =
          xworld->storePixmap(*bs0.cellimage(bno, rot, cel));
        if (&bs1.cell(bno, rot, cel) != &bs0.cell(bno, rot, cel))
          brickpixmaps[&bs1.cell(bno, rot, cel)] 
            = xworld->storePixmap(*bs1.cellimage(bno, rot, cel));
      }
    }
  }
}

void ScreenPit::paintEvent(QPaintEvent *e) {
  BBox bbox(e->rect());
  QPainter p(this);
  if (pos()!=topleft || mybg.size()!=size())
    generate();
  if (xworld)
    xworld->renderPixmap(bgpixmap, 0, 0);
  else
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
      if (xworld) {
        xworld->renderPixmap(brickpixmaps[vispit.cell(i, j)], x, y);
      } else {
        if (bb.intersect(bbox)) 
          redrawcell(&p, x, y, vispit.cell(i, j));
      }
    }
  }
  //qDebug() << "render done" << QTime::currentTime().msec();
}

void ScreenPit::poll() {
  //qDebug() << "sp poll" << QTime::currentTime().msec();
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
