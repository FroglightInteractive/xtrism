// Box.C

#include "box.H"
#include <string>

/////////////////////////////////////////////////////////////////////////////
BBox &BBox::operator|=(const BBox &bbox) {
  if (bbox.empty())
    return *this;
  if (empty())
    return (*this = bbox);
  lx=bbox.lx<?lx;
  ty=bbox.ty<?ty;
  rx=bbox.rx>?rx;
  by=bbox.by>?by;
  return *this;
  }

BBox &BBox::operator&=(const BBox &bbox) {
  lx=bbox.lx>?lx;
  ty=bbox.ty>?ty;
  rx=bbox.rx<?rx;
  by=bbox.by<?by;
  return *this;
  }

bool BBox::intersect(const BBox &bbox) const {
  return !(*this & bbox).empty();
  }

bool BBox::contains(int x, int y) const {
  return (x>=lx && x<rx && y>=ty && y<by);
  }

/////////////////////////////////////////////////////////////////////////////
Box::Box(int w, int h,
         const BoxRelPos &hrp, const BoxRelPos &vrp):
  width(w), height(h),
  x(0), y(0), hidden(0),
  hori(hrp), vert(vrp) {
  if (!hori.box || !vert.box)
    throw(string("Box: Bad link"));
  previous=hori.box->grandchild();
  next=0;
  previous->next=this;
  checkparent(false,hori);
  checkparent(true,vert);
  recalculate();
  }

Box::Box(int x1, int y1, int w, int h):
  width(w), height(h), x(x1), y(y1),
  hidden(0),
  hori(), vert(),
  previous(0), next(0) {}

Box::~Box() {
  if (next)
    throw string("Box: attempt to delete a box with children");
  if (previous)
    previous->next=0;
  }

void Box::checkparent(bool ynotx, const BoxRelPos &rp) {
  if (!isparented(rp.box))
    throw string("Box: ") + (ynotx?"vertical":"horizontal") +
      " reference is not among parents";
  }

void Box::recalculate() {
  if (previous)
    { x=newpos(false);
      y=newpos(true);
    }
  if (next)
    next->recalculate();
  }

int Box::newpos(int your_xy, int your_wh, int my_wh, const BoxRelPos &rp) {
  if (rp.box->hidden)
    your_wh=0;
  if (hidden)
    my_wh=0;
  
  int my_xy=your_xy + rp.dist;
  if (rp.your_edge == BE_Right)
    my_xy+=your_wh;
  if (rp.my_edge == BE_Right)
    my_xy-=my_wh;

  return my_xy;
  }

int Box::newpos(bool ynotx) {
  return (ynotx)?
    newpos(vert.box->y,vert.box->height,height,vert):
    newpos(hori.box->x,vert.box->width,width,hori);
  }

void Box::shift(int dx, int dy, bool recalc) {
  if (previous)
    throw string("Box: cannot shift parented box");
  x+=dx; y+=dy;
  if (recalc)
    recalculate();
  }

void Box::resize(int w, int h, bool recalc) {
  width=w; height=h;
  if (recalc)
    recalculate();
  }

void Box::reposition(const BoxRelPos &h, const BoxRelPos &v,
                bool recalc=true) {
  checkparent(false,h);
  checkparent(true,v);
  hori=h; vert=v;
  if (recalc)
    recalculate();
  }

void Box::hide(bool recalc=true) {
  hidden=true;
  if (recalc)
    recalculate();
  }

void Box::unhide(bool recalc=true) {
  hidden=false;
  if (recalc)
    recalculate();
  }

bool Box::isparented(Box *proposed_parent) {
  Box *a=previous;
  while (a)
    { if (a==proposed_parent)
        return true;
      a=a->previous;
    }
  return false;
  }

Box *Box::grandparent() {
  Box *a=this;
  while (a->previous)
    a=a->previous;
  return a;
  }

Box *Box::grandchild() {
  Box *a=this;
  while (a->next)
    a=a->next;
  return a;
  }

BBox Box::listbbox() return bbox() {
  Box *a=grandparent();
  while (a)
    { bbox|=BBox(a->x, a->y, a->x+a->width, a->y+a->height);
      a=a->next;
    }
  }

void Box::listto(int lx, int ty) {
  Box *gp=grandparent();
  gp->recalculate();
  BBox bbox(gp->listbbox());
  gp->shift(lx-bbox.left(),ty-bbox.top(),true);
  }
