// ClickGrp.C

#include "ClickGrp.H"

////////////////////////////////// Clickable /////////////////////////////////
Clickable::Clickable(class ClickGrp &grp, int w, int h,
                     const BoxRelPos &hrp, const BoxRelPos &vrp):
  Redrawable(grp.rdg,w,h,hrp,vrp), 
  canhavefocus(0), hasfocus(0), group(grp), child(0) {
  grp.add(this);
  }

Clickable::Clickable(class ClickGrp &grp, int x, int y, int w, int h):
  Redrawable(grp.rdg,x,y,w,h), 
  canhavefocus(0), hasfocus(0), group(grp), child(0) {
  group.add(this);
  }

Clickable::~Clickable() {
  group.remove(this);
  }

ClickGrp *Clickable::makechild(bool shf) {
  tthrow(child,"ClickGrp: A Clickable cannot have more than one child group");
  child=new ClickGrp(group.rdg,this,shf);
  return child;
  }

void Clickable::tfocus() {
  if (!hasfocus)
    takefocus();
  if (child)
    child->takefocus();
  }

void Clickable::lfocus() {
  if (hasfocus)
    { losefocus();
      if (child)
        child->losefocus();
    }
  }

void Clickable::focusrequest() {
  group.focusrequest(this);
  }

void Clickable::mclick(int button, bool io, int x, int y) {
  if (child && child->click(button,io, x,y))
    return;
  click(button,io,x,y);
  }

bool Clickable::kkey(int k, bool io) {
  if (child && child->key(k, io))
    return true;
  return key(k,io);
  }

////////////////////////////////// ClickGrp //////////////////////////////////
ClickGrp::ClickGrp(RedrawGrp &rdgrp, TEnv &env, bool shf):
    rdg(rdgrp), tenvp(&env), parent(0), sharedfocus(shf) {
  tenvp->reg_mouse(THandler(clickgrp_click,this));
  tenvp->reg_keyboard(THandler(clickgrp_key,this));
  }

ClickGrp::ClickGrp(RedrawGrp &rdgrp, Clickable *par, bool shf):
    rdg(rdgrp), tenvp(0), parent(par), sharedfocus(shf) {}

ClickGrp::~ClickGrp() {
  if (tenvp)
    { tenvp->dereg_mouse(THandler(clickgrp_click,this));
      tenvp->dereg_keyboard(THandler(clickgrp_key,this));
    }
  if (parent)
    { parent->forgetchild(); }
  }

void ClickGrp::focusrequest(Clickable *clk) {
  if (!sharedfocus)
    { for (list<Clickable *>::iterator c=clkl.begin(); c!=clkl.end(); ++c)
        if ((*c)!=clk)
          (*c)->lfocus();
      clk->tfocus();
    }
  parent->focusrequest();
  }

void ClickGrp::losefocus() {
  for (list<Clickable *>::iterator c=clkl.begin(); c!=clkl.end(); ++c)
    (*c)->lfocus();
  }

void ClickGrp::takefocus() {
  if (sharedfocus && !parent)
    for (list<Clickable *>::iterator c=clkl.begin(); c!=clkl.end(); ++c)
      if ((*c)->canhavefocus)
        (*c)->takefocus();
  }

void clickgrp_click(ClickGrp *grp, int button, int io, int x, int y) {
  grp->click(button,io,x,y);
  }

bool ClickGrp::click(int button, bool io, int x, int y) {
  for (list<Clickable *>::iterator c=clkl.begin(); c!=clkl.end(); ++c)
    if ((*c)->bbox().contains(x,y))
      { (*c)->mclick(button,io,x,y);
        return true;
      }
  return false;
  }

void clickgrp_key(ClickGrp *grp, int key, int io) {
  grp->key(key,io);
  }
  
bool ClickGrp::key(int key, bool io) {
  for (list<Clickable *>::iterator c=clkl.begin(); c!=clkl.end(); ++c)
    if ((*c)->hasfocus)
      if ((*c)->kkey(key,io))
        return true;
  return false;
  }
