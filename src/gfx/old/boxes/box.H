// box.H

#ifndef _box_H
#define _box_H

enum BoxEdge {
  BE_Left=0, BE_Top=0,
  BE_Right=1, BE_Bottom=1 };

class BoxRelPos {
  public:
    BoxRelPos(class Box *b=0, BoxEdge your_e=BE_Left,
              BoxEdge my_e=BE_Right, int d=0):
      box(b), my_edge(my_e), your_edge(your_e), dist(d) {}
  protected:
    class Box *box;
    BoxEdge my_edge, your_edge;
    int dist;
    friend class Box;
  };

class BBox {
  public:
    BBox(int lx1, int ty1, int rx1, int by1):
      lx(lx1), ty(ty1), rx(rx1), by(by1) {}
    BBox(): lx(0), ty(0), rx(0), by(0) {}
    
    BBox &operator|=(const BBox &bbox); // merge with other
    BBox &operator&=(const BBox &bbox); // intersect with other
    BBox operator&(const BBox &other) const
      { BBox b(*this); return b&=other; } // returns intersection
    bool empty() const { return lx>=rx || ty>=by; }
    operator bool() const { return !empty(); } // non-empty
    bool intersect(const BBox &bbox) const; // intersection isn't empty
    bool contains(int x, int y) const; // lx<=x<rx && ty<=y<by
    
    int left() const { return lx; }
    int top() const { return ty; }
    int right() const { return rx; }
    int bottom() const { return by; }
  private:
    int lx,ty, rx,by;
  };

class Box {
  public:
    Box(int w, int h,
        const BoxRelPos &hrp, const BoxRelPos &vrp);
    Box(int x, int y, int w, int h);
    ~Box();
    void recalculate();
    void shift(int dx, int dy, bool recalc=1);
    void resize(int w, int h, bool recalc=1);
    void reposition(const BoxRelPos &h, const BoxRelPos &v,
                    bool recalc=true);
    void hide(bool recalc=true);
    void unhide(bool recalc=true);
    bool isparented(Box *proposed_parent);
    Box *grandparent();
    Box *grandchild();
    BBox listbbox();
    BBox bbox() { return BBox(x,y,x+width,y+height); }
    void listto(int lx=0, int ty=0);
    int lx() { return x; }
    int ty() { return y; }
    int w() { return width; }
    int h() { return height; }
  private:
    int width, height;
    int x,y;
    bool hidden;
    BoxRelPos hori, vert;
    Box *previous, *next;

    int newpos(int your_xy, int your_wh, int my_wh, const BoxRelPos &rp);
    int newpos(bool ynotx);
    void checkparent(bool ynotx, const BoxRelPos &rp);
  };

#endif
