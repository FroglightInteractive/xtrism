// gfx/BBox.H

#ifndef _BBox_H
#define _BBox_H

#include "../basics/minmax.H"

const int BB_NoChange=-100000000; // -1e8 == -INFTY

class Point {
  public:
    Point(int x, int y): xx(x), yy(y) {}
    int x() const { return xx; }
    int y() const { return yy; }
  private:
    int xx, yy;
  };

class Area {
  public:
    Area(int w, int h): ww(w), hh(h) {}
    Area(int wh): ww(wh), hh(wh) {}
    int w() const { return ww; }
    int h() const { return hh; }
  private:
    int ww,hh;
  };

class BBox {
  public:
    BBox(int lx1, int ty1, int rx1, int by1):
      lx(lx1), ty(ty1), rx(rx1), by(by1) {}
    BBox(Point const &topleft, Area const &area):
      lx(topleft.x()), ty(topleft.y()),
      rx(topleft.x()+area.w()), by(topleft.y()+area.h()) {}
    BBox(): lx(0), ty(0), rx(0), by(0) {}

    BBox &operator|=(const BBox &bbox); // merge with other
    BBox &operator&=(const BBox &bbox); // intersect with other
    BBox operator&(const BBox &other) const
      { BBox b(*this); return b&=other; } // returns intersection
    bool empty() const { return lx>=rx || ty>=by; }
    operator bool() const { return !empty(); } // non-empty
    bool intersect(const BBox &bbox) const; // intersection isn't empty
    bool contains(int x, int y) const; // lx<=x<rx && ty<=y<by
    bool contains(const Point &p) const { return contains(p.x(),p.y()); }
    
    BBox &addmargin(int left, int top=-1, int right=-1, int bottom=-1);
    BBox &shift(int dx, int dy) { lx+=dx; rx+=dx; ty+=dy; by+=dy;
                                  return *this; }
    BBox &moveto(int x, int y) { return shift((x==BB_NoChange)?0:(x-lx),
                                              (y==BB_NoChange)?0:(y-ty)); }
    BBox &moveto(Point const &p) { return moveto(p.x(), p.y()); }
    BBox &resize(int w, int h) { rx=(w==BB_NoChange)?rx:(lx+w);
                                 by=(h==BB_NoChange)?by:(ty+h);
                                 return *this; }
    BBox &resize(Area const &a) { return resize(a.w(),a.h()); }

    int left() const { return lx; }
    int top() const { return ty; }
    int right() const { return rx; }
    int bottom() const { return by; }
    int width() const { return max(rx-lx,0); }
    int height() const { return max(by-ty,0); }
    Point topleft() const { return Point(left(), top()); }
    Area area() const { return Area(width(),height()); }
  private:
    int lx,ty, rx,by;
  };

#endif
