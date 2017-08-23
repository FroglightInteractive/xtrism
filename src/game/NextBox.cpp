// NextBox.C

#include "NextBox.H"
#include "../basics/Infty.H"
#include "../gfx/GBParent.H"
#include "../bricks/data.H"
#include "../bricks/bsprites.H"
#include "Logger.H"
#include "../bytemap/BMRecolour.H"
#include "../env/TSprite.H"

// Sleeper info: sends itself a warn whenever contents must be redrawn

NextBox::NextBox(GBParent *p, SBrickData const &sbd0, BrickSprites const &bs0,
                 SharedBG &sbg, class PollServer &syncserv,
                   bool myid0, Logger *logger0):
  GBox(p, Area((sbd0.maxlines() + 1) * bs0.size())),
  BGSharer(sbg),
  Sleeper(syncserv),
  sbd(sbd0), bs(bs0),
  bno(-1), rot(0),
  myid(myid0), logger(logger0) {
}

NextBox::~NextBox() {
}

void NextBox::update(int bno0, int rot0) {
  if (logger)
    logger->newnext(myid, bno0, rot0);
  bno = bno0;
  rot = rot0;
  warn();
}

void NextBox::poll() {
  redraw(bbox());
}

bool NextBox::draw(Point const &origin, class ByteMap *bm) {
  int bw = 1 + width() / 50;
  recolour_circle(bm, Point(left() + width() / 2 - origin.x(),
                            top() + height() / 2 - origin.y()),
                  width() / 2 - bw - 1, .5, 0,
                  width() / 2 - 1, -64);
  return true;
}

void NextBox::redraw(const BBox &bb) {
  BGSharer::redraw(bb);
  if (bno >= 0) {
    BrickData const &bd = sbd.brick(bno, rot);
    int size = bs.size();
    int x0 = left() + size / 2;
    int y0 = top() + size / 2;
    float xave = 0;
    float yave = 0;
    for (unsigned int i = 0; i < bd.cells(); i++) {
      xave += bd.x(i);
      yave += bd.y(i);
    }
    x0 -= int(size * (xave / bd.cells() - 1.5));
    y0 -= int(size * (yave / bd.cells() - 1.5));
    for (unsigned int i = 0; i < bd.cells(); i++)
      bs.cell(bno, rot, i).put(x0 + bd.x(i) * size, y0 + bd.y(i) * size);
  }
}