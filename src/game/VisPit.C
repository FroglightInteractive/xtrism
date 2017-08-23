// VisPit.C

#include "VisPit.H"
#include "../env/TSprite.H"
#include "../bricks/data.H"
#include "../bricks/bsprites.H"
#include "Logger.H"
#include "../basics/dbx.H"
#include "../basics/Throw.H"

// Waker info: sends warn() whenever contents change

VisPit::VisPit(int w, int h,
               SBrickData const &sbd0,
               BrickSprites const &bsp0, BrickSprites const &bsp1,
               Logger *logger0):
  wid(w), hei(h), sbd(sbd0), bsp(bsp0), bsp2(bsp1), logger(logger0) {
  map = new TSprite const *[wid * hei];
  chmap = new word[hei];
  int bno, rot, cel;
  for (int y = 0; y < hei; y++) {
    chmap[y] = 0;
    for (int x = 0; x < wid; x++)
      refcell(x, y) = 0;
  }
  op_inuse[0] = op_inuse[1] = 0;
}

VisPit::~VisPit() {
  delete[] map;
  delete[] chmap;
}

bool VisPit::changed(int x, int y) const {
  return (chmap[y] & (1 << x)) != 0;
}

bool VisPit::changedline(int y) const {
  return chmap[y] != 0;
}

void VisPit::resetchanged() {
  for (int y = 0; y < hei; y++)
    chmap[y] = 0;
}

void VisPit::setchanged(int x, int y) {
  chmap[y] |= 1 << x;
}

void VisPit::setchanged(int y) {
  chmap[y] = ~0;
}

void VisPit::addrem(FBPos const &pos, bool addnotrem, bool usesec) {
  BrickData const &brk = sbd.brick(pos.bno, pos.rot);
  for (int c = 0; c < brk.cells(); c++) {
    int x = pos.x + brk.x(c) - 4; // experimental !!!!!
    int y = pos.y + brk.y(c);
    if (x >= 0 && x < wid && y >= 0 && y < hei) {
      refcell(x, y) = addnotrem ? &(usesec ? bsp2 : bsp).cell(pos.bno,
                                                              pos.rot, c) : 0;
      setchanged(x, y);
    } else
      dbx(0, "VisPit::addrem: illegal address (%i,%i)", x, y);
  }
}

void VisPit::copyline(int dst, int src) {
  for (int x = 0; x < wid; x++)
    if (cell(x, dst) != cell(x, src)) {
      refcell(x, dst) = cell(x, src);
      setchanged(x, dst);
    }
}

void VisPit::zeroline(int dst) {
  for (int x = 0; x < wid; x++)
    if (cell(x, dst) != 0) {
      refcell(x, dst) = 0;
      setchanged(x, dst);
    }
}

void VisPit::pudding(int y, int dir) {
  if (logger)
    logger->pudding(y, dir);
  if (dir > 0) {
    for (int l = y; l > 0; l--)
      copyline(l, l - 1);
    zeroline(0);
  } else {
    for (int l = y; l < hei - 1; l++)
      copyline(l, l + 1);
    zeroline(hei - 1);
  }
  for (int i = 0; i < 2; i++)
    if (op_inuse[i]) {
      FBPos &p = oldpos[i];
      if ((p.y + 1 < y && dir > 0) || (p.y + 2 > y && dir < 0)) {
        p.y += dir;   /* addbrick(p,i,0); */
      }                                        // EXPERIMENTAL !!!
    }
  sendwarn();
}

void VisPit::clear() {
  for (int i = 0; i < hei; i++)
    for (int j = 0; j < wid; j++) {
      refcell(j, i) = 0;
      setchanged(j, i);
    }
  sendwarn();
}

void VisPit::addbrick(FBPos const &pos, int id, bool definitive) {
  if (logger)
    logger->addbrick(pos, id, definitive);
  if (id < 0) {
    addrem(pos, true);
    sendwarn();
    return;
  }
  tthrow(id > 1, "VisPit: Illegal ID");
  if (op_inuse[id])
    addrem(oldpos[id], false, id > 0);
  op_inuse[id] = false;
  addrem(pos, true, id > 0);
  if (!definitive) {
    op_inuse[id] = true;
    oldpos[id] = pos;
  }
  sendwarn();
}

void VisPit::rembrick(int id) {
  if (logger)
    logger->rembrick(id);
  tthrow(id < 0 || id > 1, "VisPit: Illegal ID");
  fthrow(op_inuse[id], "VisPit: ID not in use");
  addrem(oldpos[id], false);
  op_inuse[id] = false;
  sendwarn();
}

void VisPit::rembrick(FBPos const &pos) {
  if (logger)
    logger->rembrick(pos);
  addrem(pos, false);
}

int VisPit::cellsize() const {
  return bsp.size();
}