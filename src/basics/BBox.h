// gfx/BBox.H

#ifndef _BBox_H
#define _BBox_H

#include <QRect>
#include "../basics/minmax.h"

const int BB_NoChange = -100000000; // -1e8 == -INFTY

class BBox: public QRect {
public:
  BBox(int lx, int ty, int rx, int by):
    QRect(lx, ty, rx-lx, by-ty) {
  }
  BBox(QRect rct): QRect(rct) { }
  BBox(QPoint const &topleft, QSize const &area):
    QRect(topleft, area) {
  }
  BBox(): QRect(0,0,0,0) {
  }
  BBox &operator=(QRect const &r) { QRect::operator=(r); return *this; }
  bool empty() const {
    return isEmpty();
  }
  operator bool() const { // non-empty
    return !empty();
  }                                           
  bool intersect(const BBox &bbox) const {
    return intersects(bbox);
  }
  BBox &addmargin(int left, int top=-1, int right=-1, int bottom=-1) {
    if (top<0)
      top = left;
    if (right<0)
      right = left;
    if (bottom<0)
      bottom = top;
    *this = marginsAdded(QMargins(left, top, right, bottom));
    return *this;
  }
  BBox &shift(int dx, int dy) {
    translate(dx, dy);
    return *this;
  }
  BBox &moveto(int x, int y) {
    return shift((x == BB_NoChange) ? 0 : (x - left()),
                 (y == BB_NoChange) ? 0 : (y - top()));
  }
  BBox &moveto(QPoint const &p) {
    return moveto(p.x(), p.y());
  }
  BBox &resize(int w, int h) {
    if (w!=BB_NoChange)
      setWidth(w);
    if (h!=BB_NoChange)
      setHeight(h);
    return *this;
  }
  BBox &resize(QSize const &a) {
    return resize(a.width(), a.height());
  }
};

#endif
