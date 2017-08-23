// WishList.C

#include "WishList.h"
#include "../basics/dbx.h"
#include "../sound/Sounds.h"

////////////////////////////////// WLResult //////////////////////////////////

WLResult & WLResult::operator|=(const FBHResult &fbh) {
  moved |= fbh.moved;
  othermoved |= fbh.othermoved;
  return *this;
}

WLResult &WLResult::operator|=(const FBVResult &fbv) {
  moved |= fbv.moved;
  gonedown |= fbv.moved && !fbv.landed;
  othermoved |= fbv.othermoved;
  landed |= fbv.landed;
  dropped |= fbv.dropped;
  return *this;
}

////////////////////////////////// WishList //////////////////////////////////

WishList::WishList(FallBrick &fb0, Kronos &kr0):
  fb(fb0), kronos(kr0) {
  clearflags();
}

bool WishList::logkey(BufferCode b) {
  dbx(2, "WishList::logkey %i", b);
  switch (b) {
  case BC_iLeft:
    movemark.left = true;
    return true;

  case BC_oLeft:
    movemark.zleft &= 2;
    movemark.left = false;
    return true;

  case BC_oZLeft:
    if (movemark.zleft == 3)
      movemark.left = true;
    movemark.zleft &= 2;
    return true;

  case BC_iZLeft:
    movemark.zleft = 3;
    return true;

  case BC_iRight:
    movemark.right = true;
    return true;

  case BC_oRight:
    movemark.zright &= 2;
    movemark.right = false;
    return true;

  case BC_oZRight:
    if (movemark.zright == 3)
      movemark.right = true;
    movemark.zright &= 2;
    return true;

  case BC_iZRight:
    movemark.zright = 3;
    return true;

  case BC_iRotCW:
    movemark.rotcw = true;
    return true;

  case BC_iRotCCW:
    movemark.rotccw = true;
    return true;

  case BC_iDrop:
    movemark.drop = true;
    return true;

  default:
    return false;          // not handling any of the others
  }
}

void WishList::clearflags() {
  movemark.left = movemark.right = movemark.zleft = movemark.zright
                                                      = movemark.rotcw
                                                          = movemark.rotccw
                                                              = movemark.drop
                                                                  = 0;
  prot.invol_land = prot.unexp_x = 0;
}

void WishList::warn() {
  sounds->warn();
  // make a warning sound...
}

WLResult WishList::poll() {
  WLResult wlr;

  // left and zleft
  if (movemark.zleft) {
    FBHResult fbr = fb.zleft();
    prot.unexp_x = fbr.hitother;
    movemark.zleft &= 1;
    wlr |= fbr;
  } else if (movemark.left) {
    FBHResult fbr = fb.left();
    prot.unexp_x = fbr.hitother || fbr.shooshed;
    if (fbr.moved)
      movemark.left = 0;
    wlr |= fbr;
  }

  // right and zright
  if (movemark.zright) {
    FBHResult fbr = fb.zright();
    prot.unexp_x = fbr.hitother;
    movemark.zright &= 1;
    wlr |= fbr;
  } else if (movemark.right) {
    FBHResult fbr = fb.right();
    prot.unexp_x = fbr.hitother || fbr.shooshed;
    if (fbr.moved)
      movemark.right = 0;
    wlr |= fbr;
  }

  // rotcw
  if (movemark.rotcw) {
    FBHResult fbr = fb.rotate(+1);
    if (fbr.moved)
      movemark.rotcw = 0;
    wlr |= fbr;
  }

  // rotccw
  if (movemark.rotccw) {
    FBHResult fbr = fb.rotate(-1);
    if (fbr.moved)
      movemark.rotccw = 0;
    wlr |= fbr;
  }

  // drop
  if (movemark.drop) {
    movemark.drop = false;
    if (prot.unexp_x) {
      warn();
      prot.unexp_x = 0;
    } else if (prot.invol_land && !kronos.safeival()) {
      warn();
      prot.invol_land = 0;
    } else {
      prot.invol_land = 0;
      wlr |= fb.drop();
      if (!wlr.landed)
        warn();     // other must've been in the way...
    }
  } else if (kronos.mustigodown()) {
    wlr |= fb.godown();
    if (!wlr.gonedown)
      kronos.notgonedown();
  }
  if (wlr.landed)
    landed(wlr.dropped);
  return wlr;
}

void WishList::landed(bool dropped) {
  prot.unexp_x = false;
  prot.invol_land = !dropped;
}