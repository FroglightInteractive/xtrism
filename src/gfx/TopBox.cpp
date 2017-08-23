// TopBox.C

#include <string>
#include "TopBox.H"
#include "../env/TTime.H"
#include <algorithm>
#include "../basics/Throw.H"

void TopBox::tredraw(int lx, int ty, int w, int h) {
  BBox b(lx, ty, lx + w, ty + h);
  b &= bbox();
  if (b)
    redraw(b);
}

bool TopBox::inme(const Point &p) {
  bool in(bbox().contains(p));
  if (in != containspointer)
    enterleave(in, p);
  containspointer = in;
  return in;
}

void TopBox::tmotion(int x, int y) {
  Point p(x, y);
  if (inme(p))
    motion(p);
}

void TopBox::tenterleave(bool enl) {
  inme(env.pointer());
}

void TopBox::tmbc(int button, bool in_not_out, int x, int y) {
  Point p(x, y);
  if (claimed_mouse) {
    claimed_mouse->mouse(button, in_not_out, p);
    if (!button)
      claimed_mouse = 0;
  } else {
    if (inme(p))
      mouse(button, in_not_out, p);
  }
}

void TopBox::claim_mouse(GBox *b) {
  tthrow(claimed_mouse, "TopBox: double claim_mouse()");
  claimed_mouse = b;
}

void TopBox::rel_mouse(GBox *b) {
  tthrow(claimed_mouse && b != claimed_mouse,
         "TopBox: unexpected rel_mouse()");
  claimed_mouse = 0;
}

void TopBox::tkey(int kc, bool in_not_out) {
  key(kc, in_not_out);
}

TopBox::TopBox(TEnv &tenv, BBox const &bbox, bool sharesel):
  GBParent(0, bbox.width() ? bbox.width() : tenv.width(),
           bbox.height() ? bbox.height() : tenv.height(), sharesel),
  env(tenv), containspointer(false), claimed_mouse(0) {
  moveto(bbox.topleft());
  env.reg_keyboard(this);
  env.reg_mml(this);
  env.reg_redraw(this);
  env.reg_poll(this);
  env.reg_sync(this);
  dbx(-980809, "TopBox: poll=%p sync=%p", &poll, &sync);
}

TopBox::~TopBox() {
  env.dereg_sync(this);
  env.dereg_poll(this);
  env.dereg_keyboard(this);
  env.dereg_mml(this);
  env.dereg_redraw(this);
}

void TopBox::reenter(bool justleave, bool forgetstate) {
  if (forgetstate)
    containspointer = false;
  if (containspointer) {
    enterleave(false, Point(-1, -1));
    containspointer = false;
  }
  if (!justleave)
    inme(env.pointer());
}