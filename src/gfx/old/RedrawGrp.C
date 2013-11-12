// RedrawGrp.C

#include "RedrawGrp.H"

/////////////////////////////// Redrawable ///////////////////////////////////
Redrawable::Redrawable(class RedrawGrp &grp, int w, int h,
                       const BoxRelPos &hrp, const BoxRelPos &vrp):
  Box(w,h,hrp,vrp), TGFX(grp.tgfx()), group(grp) {
  grp.add(this);
  }

Redrawable::Redrawable(class RedrawGrp &grp, int x, int y, int w, int h):
  Box(x,y,w,h), TGFX(grp.tgfx()), group(grp) {
  group.add(this);
  }

Redrawable::~Redrawable() {
  group.remove(this);
  }

////////////////////////////// RedrawGrp /////////////////////////////////////
RedrawGrp::RedrawGrp(TEnv &env): tenv(env) {
  tenv.reg_redraw(THandler(redrawgrp_redraw,this));
  }

RedrawGrp::~RedrawGrp() {
  tenv.dereg_redraw(THandler(redrawgrp_redraw,this));
  }

void redrawgrp_redraw(RedrawGrp *grp, int lx, int ty, int w, int h) {
  grp->redraw(BBox(lx,ty,lx+w,ty+h));
  }

void RedrawGrp::redraw(const BBox &bbox) {
  for (list<Redrawable *>::iterator r=rdwl.begin(); r!=rdwl.end(); ++r)
    { BBox a=bbox & (*r)->bbox();
      if (a)
        (*r)->redraw(a);
    }
  }
