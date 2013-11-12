// gfx/BBox.C

#include "BBox.H"

//////////////////////////////////// BBox ////////////////////////////////////
BBox &BBox::operator|=(const BBox &bbox) {
  if (bbox.empty())
    return *this;
  if (empty())
    return (*this = bbox);
  makeatmost(lx,bbox.lx);
  makeatmost(ty,bbox.ty);
  makeatleast(rx,bbox.rx);
  makeatleast(by,bbox.by);
  return *this;
  }

BBox &BBox::operator&=(const BBox &bbox) {
  makeatleast(lx,bbox.lx);
  makeatleast(ty,bbox.ty);
  makeatmost(rx,bbox.rx);
  makeatmost(by,bbox.by);
  return *this;
  }

bool BBox::intersect(const BBox &bbox) const {
  return !(*this & bbox).empty();
  }

bool BBox::contains(int x, int y) const {
  return (x>=lx && x<rx && y>=ty && y<by);
  }

BBox &BBox::addmargin(int left, int top, int right, int bottom) {
  if (top<0)
    top=left;
  if (right<0)
    right=left;
  if (bottom<0)
    bottom=top;
  lx-=left; ty-=top;
  rx+=right; by+=bottom;
  return *this;
  }
