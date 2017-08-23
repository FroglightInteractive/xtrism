// gfx/GBox.C

#include <string>
#include "GBox.H"
#include "GBParent.H"
#include "TopBox.H"
#include "../basics/Throw.H"

#include "../basics/dbx.H"

//////////////////////////////////// GBox ////////////////////////////////////
GBox::GBox(GBParent *par0, Area const &area):
  bb(Point(0, 0), area),
  par(par0),
  focus(0), selected(0) {
  dbx(1, "GBox: %p", this);
}

GBox::GBox(GBParent *par0, int w, int h):
  bb(0, 0, w, h),
  hasp(0), par(par0),
  focus(0), selected(0) {
  dbx(1, "GBox: %p", this);
}

GBox::~GBox() {
  dbx(1, "~GBox: %p", this);
  if (hasp && par)
    par->losechild(this);
}

void GBox::takefocus() {
  // athrow("GBox::takefocus: not parented");
  if (par)
    if (!focus)
      par->takefocus_notify(this);
  focus = true;
}

void GBox::losefocus() {
  if (par)
    if (focus)
      par->losefocus_notify(this);
  focus = false;
}

void GBox::takeselect() {
  if (par)
    if (!selected)
      par->takeselect_notify(this);
  selected = true;
}

void GBox::loseselect() {
  if (par)
    if (selected)
      par->loseselect_notify(this);
  selected = false;
}

TopBox *GBox::topbox() {
  return par ? par->topbox() : 0;
}

TGFX const &GBox::tgfx() {
  TopBox *t = topbox();
  fthrow(t, "GBox::tgfx: no TopBox as ancestor");
  return t->tgfx();
}