// gfx/GBox.H

#ifndef _GBox_H
#define _GBox_H

#include "../basics/BBox.h"

class GBox {
public:
  // constructors & destructors
  GBox(class GBParent *par0, Area const &area);
  GBox(class GBParent *par0, int w, int h);
  virtual ~GBox();
public:
  // functions
  int left() const {
    return bb.left();
  }
  int top() const {
    return bb.top();
  }
  int right() const {
    return bb.right();
  }
  int bottom() const {
    return bb.bottom();
  }
  int width() const {
    return bb.width();
  }
  int height() const {
    return bb.height();
  }
  Point topleft() const {
    return bb.topleft();
  }
  Area area() const {
    return bb.area();
  }
  BBox const &bbox() const {
    return bb;
  }
  class GBParent *parent() const {
    return par;
  }
  virtual class TopBox *topbox();
  virtual class TGFX const &tgfx();
  void draw() {
    redraw(bb);
  }

  bool isselected() const {
    return selected;
  }
  bool hasfocus() const {
    return focus;
  }
public:
  // virtual functions
  virtual void redraw(const BBox &bbox) = 0;
  virtual void motion(const Point & /*newpos*/) {
  }
  /* newpos is guaranteed to lie within this GBox's bbox, EXCEPT for
     the toplevel (of a popup). */
  virtual void enterleave(bool /*in_not_out*/, const Point & /*newpos*/) {
  }
  /* Iff in_not_out, newpos is guaranteed to lie within this GBox's
     bbox, EXCEPT for the toplevel (of a popup). */
  virtual bool mouse(int /*button*/, bool /*in_not_out*/,
                     const Point & /*pos*/) {
    return false;
  }
  /* you will never receive a mouse() call before receiving an
     enterleave(true,xxx) call, or after an enterleave(false,xxx) call.
     ! This does NOT apply to a GBox (or more probably GBParent) that is
     the toplevel (of a popup). */
  virtual bool key(int /*key*/, bool /*in_not_out*/) {
    return false;
  }
  virtual void shift(int dx, int dy) {
    bb.shift(dx, dy);
  }
  virtual void forcedlosefocus() {
    focus = false;
  }
  virtual void forcedloseselect() {
    selected = false;
  }
  virtual bool offerfocus() {
    return false;
  }
  /* To accept the offer, call takefocus() and return true. */
  virtual bool offerselect() {
    return false;
  }
  /* To accept the offer, call takeselect() and return true. */
protected:
  // for descendants
  void resize(Area const &a) {
    bb.resize(a);
  }
  void resize(int w, int h) {
    bb.resize(w, h);
  }
  void takefocus(), losefocus();
  void takeselect(), loseselect();
protected:
  // for parent
  void moveto(int x, int y) {
    shift((x == BB_NoChange) ? 0 : x - left(),
          (y == BB_NoChange) ? 0 : y - top());
  }
  void moveto(Point const &p) {
    moveto(p.x(), p.y());
  }
  bool parent_says_hello(class GBParent *p) {
    if (p == par) hasp = true;
    return p == par;
  }
  void parent_says_goodbye(class GBParent *p) {
    if (p == par) hasp = false;
  }
  friend class GBParent;
  friend class GBChildInfo;
private:
  // data
  BBox bb;
  bool hasp;
  class GBParent *par;
private:
  // data
  bool focus, selected;
};

#endif