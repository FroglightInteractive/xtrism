// GBParent.H

#ifndef _GBParent_H
#define _GBParent_H

#include "GBox.H"
#include <list>
#include "../basics/Throw.H"

class GBPos {
public:
  GBPos(GBox *whence0, int dist0): whence(whence0), dist(dist0) {
  }
  GBPos(GBox *whence0): whence(whence0), dist(-1) {
  }
  GBPos(int x=0): whence(0), dist(0) {
    if (x != 0) athrow("GBPos(int x!=0)");
  }
protected:
  GBox *whence;
  int dist;
  friend class GBParent;
  friend class GBChildInfo;
  friend class GBDPos;
};

const bool GBP_Min = 0, GBP_Max = 1, GBP_Hori = 0, GBP_Vert = 1;

class GBDPos {
public:
  GBDPos(GBPos const &min0, GBPos const &max0):
    min(min0), max(max0), placed(0) {
  }
  GBDPos(): placed(0) {
  }
  GBPos &operator[](bool x) {
    return x ? max : min;
  }
protected:
  GBPos min, max;
  bool placed;
  friend class GBChildInfo;
  friend class GBParent;
};

class GBChildInfo {
public:
  GBChildInfo(GBox *child0,
              GBPos const &left0, GBPos const &top0,
              GBPos const &right0, GBPos const &bottom0,
              bool ispopup0=false,
              bool killwhendying0=false):
    child(child0), hori(left0, right0), vert(top0, bottom0),
    ispopup(ispopup0), killwhendying(killwhendying0) {
  }
  GBChildInfo(): child(0), ispopup(0), killwhendying(0) {
  }
  GBDPos &operator[](bool vnoth) {
    return vnoth ? vert : hori;
  }
protected:
  bool placed() {
    return hori.placed && vert.placed;
  }
  void unplace() {
    hori.placed = vert.placed = false;
  }

  int size(bool vnoth) {
    return vnoth ? child->height() : child->width();
  }
  int dist(bool vnoth, bool side) {
    return e(vnoth, side).dist;
  }
  GBox *whence(bool vnoth, bool side) {
    return e(vnoth, side).whence;
  }
  int edge(bool vnoth, bool side, bool theirside);
  GBChildInfo &sibling(bool vnoth, bool side, class GBParent *par);
  bool placed(bool vnoth) {
    return (*this)[vnoth].placed;
  }
  bool edge_fixed(bool vnoth, bool side, class GBParent *par);
  bool double_depend(bool vnoth);
  bool fixed_depend(bool vnoth, class GBParent *par);
  bool forms_chain(bool vnoth, class GBParent *par);
  void undepend(GBChildInfo &wh);
  void moveto(bool vnoth, int xory);
private:
  GBDPos &d(bool vnoth) {
    return vnoth ? vert : hori;
  }
  GBPos &e(bool vnoth, bool side) {
    return d(vnoth)[side];
  }
  bool cont_chain(bool vnoth, class GBParent *par);
  bool reciprocal_depend(bool vnoth, GBChildInfo *other);
  GBox &sib(bool vnoth, bool side);
protected:
  GBox *child;
  GBDPos hori, vert;
  bool ispopup, killwhendying;
  friend class GBParent;
};

class GBParent: public GBox {
public:
  GBParent(GBParent *par0, Area const &area, bool sharesel=false);
  GBParent(GBParent *par0, int w, int h, bool sharesel=false);
  virtual ~GBParent();
  void newchild(GBChildInfo const &ch);
  void newchild(GBox *gb, const GBPos &left, const GBPos &top,
                const GBPos &right, const GBPos &bottom);
  void placechildren();
public:
  // virtual functions
  virtual void redraw(const BBox &bbox);
  virtual void motion(const Point &newpos);
  virtual void enterleave(bool in_not_out, const Point &newpos);
  virtual bool mouse(int button, bool in_not_out, const Point &pos);
  virtual bool key(int key, bool in_not_out);

  virtual void forcedlosefocus() {
    focus = false;
  }
  virtual void forcedloseselect() {
    selected = false;
  }
  virtual bool offerfocus();
  virtual bool offerselect();

  void shift(int dx, int dy);
protected:
  // for GBox
  void losechild(GBox *ch);
  void takefocus_notify(GBox *ch);
  void losefocus_notify(GBox *ch);
  void takeselect_notify(GBox *ch);
  void loseselect_notify(GBox *ch);
  friend class GBox;
protected:
  GBChildInfo *find(GBox *ch) {
    list<GBChildInfo>::iterator i = findit(ch);
    return i == children.end() ? 0 : &*i;
  }
  friend class GBChildInfo;
private:
  GBox *whereis(Point const &p);
  list<GBChildInfo>::iterator findit(GBox *ch);
  void placechildren_(bool vnoth);
  void pc_one(bool vnoth, GBChildInfo &ch);
  int pc_chain(bool vnoth, GBChildInfo &start, bool freemax);
  bool pc_get_minmax(bool vnoth, bool side, GBChildInfo &ch, int &res);
private:
  bool sharedselect;
  list<GBChildInfo> children;
  GBox *mouse_is_in;
  GBox *focus_child, *selected_child;
  bool autoresize;
};

#endif