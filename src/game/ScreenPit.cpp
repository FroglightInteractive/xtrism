// ScreenPit.C

#include <math.h>

#include "ScreenPit.h"
#include "../env/TDrawCol.h"
#include "../gfx/GBParent.h"
#include "../env/TSprite.h"
#include "../env/TImage.h"
#include "../bytemap/BMRecolour.h"

ScreenPit::ScreenPit(GBParent *p, VisPit &vp, SharedBG &sbg,
                     class PollServer &syncserv):
  GBox(p, Area(vp.cellsize() * vp.width(), vp.cellsize() * vp.height())),
  BGSharer(sbg),
  Sleeper(syncserv),
  vispit(vp) {
  regwaker(&vp);
}

ScreenPit::~ScreenPit() {
}

bool ScreenPit::draw(Point const &origin, class ByteMap *bm) {
  int bw = width() / 100 + 1;
  BBox bb = bbox();
  bb.shift(-origin.x(), -origin.y());
  int W = vispit.width();
  int dx = vispit.cellsize();
  bb.resize(1, BB_NoChange);
  for (int x = 0; x < W * dx; x++) {
    double phase = x * 3.141592 / dx;
    recolour_rectangle(bm, bb, .5 + .15 * cos(phase), 0);
    bb.shift(1, 0);
  }
  bb = bbox();
  bb.shift(-origin.x(), -origin.y());
  recolour_rect_edges(bm, bb, bw, -64);
  return true;
}

void ScreenPit::redraw(BBox const &bbox) {
  int dx = vispit.cellsize();
  int dy = dx;
  int x0 = left();
  int y0 = top();
  for (int j = 0; j < vispit.height(); j++) {
    int y = y0 + dy * j;
    for (int i = 0; i < vispit.width(); i++) {
      int x = x0 + dx * i;
      BBox bb(x, y, x + dx, y + dy);
      if (bb.intersect(bbox)) {
        TSprite const *tsp(vispit.cell(i, j));
        redrawcell(x, y, tsp);
      }
    }
  }
}

void ScreenPit::poll() {
  int dx = cellsize();
  int dy = cellsize();
  int x0 = left();
  int y0 = top();
  for (int j = 0; j < vispit.height(); j++)
    if (vispit.changedline(j)) {
      int y = y0 + dy * j;
      for (int i = 0; i < vispit.width(); i++)
        if (vispit.changed(i, j)) {
          int x = x0 + dx * i;
          TSprite const *const tsp = vispit.cell(i, j);
          redrawcell(x, y, tsp);
        }
    }
  vispit.resetchanged();
}

void ScreenPit::redrawcell(int x, int y, TSprite const *tsp) const {
  if (tsp)
    tsp->put(x, y);
  else
    BGSharer::redraw(BBox(Point(x, y), Area(cellsize(), cellsize())));
}