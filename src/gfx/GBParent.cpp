// GBParent.C

#include <string>
#include "GBParent.h"
#include "../basics/Throw.h"
#include <vector>
#include "../basics/dbx.h"

///////////////////////////////// GBChildInfo ////////////////////////////////
GBox &GBChildInfo::sib(bool vnoth, bool side) {
  GBox *gb = e(vnoth, side).whence;
  if (!gb)
    athrow("GBChildInfo: side not bound");
  return *gb;
}

int GBChildInfo::edge(bool vnoth, bool side, bool theirside) {
  GBox &gb = sib(vnoth, side);
  return vnoth
         ? (theirside ? gb.bottom() : gb.top())
         : (theirside ? gb.right() : gb.left());
}

GBChildInfo &GBChildInfo::sibling(bool vnoth, bool side, GBParent *par) {
  GBox &gb = sib(vnoth, side);
  if (&gb == par)
    athrow("GBChildInfo::sibling: side bound to parent");
  GBChildInfo *gbci = par->find(&gb);
  if (!gbci)
    athrow("GBChildInfo::sibling: parent doesn't recognize side");
  return *gbci;
}

bool GBChildInfo::edge_fixed(bool vnoth, bool side, GBParent *par) {
  GBox *ed = whence(vnoth, side);
  return ed == par || (ed && sibling(vnoth, side, par).placed(vnoth));
}

bool GBChildInfo::double_depend(bool vnoth) {
  return whence(vnoth, 0) && whence(vnoth, 1);
}

bool GBChildInfo::fixed_depend(bool vnoth, GBParent *par) {
  return double_depend(vnoth)
         ? (edge_fixed(vnoth, 0, par) && edge_fixed(vnoth, 1, par))
         : (edge_fixed(vnoth, 0, par) || edge_fixed(vnoth, 1, par));
}

bool GBChildInfo::forms_chain(bool vnoth, GBParent *par) {
  GBox *min = whence(vnoth, 0);
  GBox *max = whence(vnoth, 1);
  if (!(min == par || (min && sibling(vnoth, 0, par).placed(vnoth))) || !max)
    return false;

  if (max == par)
    return true;

  GBChildInfo &s = sibling(vnoth, 1, par);
  return s.placed(vnoth)
         || (reciprocal_depend(vnoth, &s) && s.cont_chain(vnoth, par));
}

bool GBChildInfo::reciprocal_depend(bool vnoth, GBChildInfo *other) {
  return whence(vnoth, 1) == other->child
         && other->whence(vnoth, 0) == child;
}

bool GBChildInfo::cont_chain(bool vnoth, GBParent *par) {
  GBox *max = whence(vnoth, 1);
  if (max == par)
    return true;

  if (!max)
    return false;

  GBChildInfo &s = sibling(vnoth, 1, par);
  return s.placed(vnoth)
         || (reciprocal_depend(vnoth, &s) && s.cont_chain(vnoth, par));
}

void GBChildInfo::undepend(GBChildInfo &wh) {
  GBox *whb = wh.child;
  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 2; j++)
      if (whence(i, j) == whb)
        e(i, j) = GBPos(0);
  // should - perhaps - compute new dependency from dying sibling's.
}

void GBChildInfo::moveto(bool vnoth, int xory) {
  if (vnoth)
    child->moveto(BB_NoChange, xory);
  else
    child->moveto(xory, BB_NoChange);
  d(vnoth).placed = true;
}

////////////////////////////////// GBParent //////////////////////////////////
GBParent::GBParent(GBParent *par0, Area const &area, bool sharesel):
  GBox(par0, area),
  sharedselect(sharesel),
  mouse_is_in(0),
  focus_child(0), selected_child(0) {
  dbx(1, "GBParent %p", this);
  autoresize = bbox().empty();
}

GBParent::GBParent(GBParent *par0, int w, int h, bool sharesel):
  GBox(par0, w, h),
  sharedselect(sharesel),
  mouse_is_in(0),
  focus_child(0), selected_child(0) {
  dbx(1, "GBParent %p", this);
  autoresize = bbox().empty();
}

GBParent::~GBParent() {
  dbx(2, "~GBParent %p", this);
  list<GBChildInfo>::iterator i, j;
  i = children.begin();
  while (i != children.end()) {
    j = i;
    ++i;
    dbx(2, "~GBP: %p child %p - %s", this, (*j).child,
        (*j).killwhendying ? "OK" : "BAD");
    if ((*j).killwhendying)
      delete (*j).child;
    // else
    // throw "GBParent: dying while children are present";
  }
}

void GBParent::newchild(GBChildInfo const &ch) {
  if (ch.child->parent() != this)
    athrow("GBParent::newchild: child doesn't know me");
  children.push_back(ch);
  dbx(2, "GBP %p: new %p", this, ch.child);
}

void GBParent::newchild(GBox *gb, const GBPos &left, const GBPos &top,
                        const GBPos &right, const GBPos &bottom) {
  if (!gb->parent_says_hello(this))
    athrow("GBParent::newchild: child doesn't know me");
  children.push_back(GBChildInfo(gb, left, top, right, bottom));
  dbx(2, "GBP %p: new %p", this, gb);
}

void GBParent::losechild(GBox *ch) {
  list<GBChildInfo>::iterator chi = findit(ch);
  if (chi == children.end())
    athrow("GBParent: attempt to lose a non-existing child");
  for (list<GBChildInfo>::iterator i = children.begin();
       i != children.end();
       ++i)
    (*i).undepend(*chi);
  children.erase(chi);
  ch->parent_says_goodbye(this);
  dbx(2, "GBP %p: del %p", this, ch);
}

void GBParent::takefocus_notify(GBox *ch) {
  takefocus();
  if (focus_child && focus_child != ch)
    focus_child->forcedlosefocus();
  focus_child = ch;
}

void GBParent::losefocus_notify(GBox */*ch*/) {
  focus_child = 0;
  offerfocus();
}

bool GBParent::offerfocus() {
  for (list<GBChildInfo>::iterator i = children.begin();
       i != children.end(); ++i)
    if ((*i).child->offerfocus()) {
      takefocus();
      return true;
    }
  return false;
}

void GBParent::takeselect_notify(GBox *ch) {
  takeselect();
  if (selected_child && selected_child != ch)
    selected_child->forcedloseselect();
  selected_child = ch;
}

void GBParent::loseselect_notify(GBox */*ch*/) {
  selected_child = 0;
  offerselect();
}

bool GBParent::offerselect() {
  for (list<GBChildInfo>::iterator i = children.begin();
       i != children.end(); ++i)
    if ((*i).child->offerselect()) {
      takeselect();
      return true;
    }
  return false;
}

void GBParent::redraw(const BBox &bbox) {
  for (list<GBChildInfo>::iterator i = children.begin();
       i != children.end(); ++i) {
    BBox bb(bbox);
    bb &= (*i).child->bbox();
    if (bb)
      (*i).child->redraw(bb);
  }
}

void GBParent::motion(const Point &newpos) {
  bool move = true;
  if (mouse_is_in) {
    if ((move = !mouse_is_in->bbox().contains(newpos)))
      mouse_is_in->enterleave(false, newpos);
    else
      mouse_is_in->motion(newpos);
  }
  if (move)
    if ((mouse_is_in = whereis(newpos)), mouse_is_in)
      mouse_is_in->enterleave(true, newpos);
}

void GBParent::enterleave(bool in_not_out, const Point &newpos) {
  if (in_not_out) {
    mouse_is_in = whereis(newpos);
    if (mouse_is_in)
      mouse_is_in->enterleave(true, newpos);
  } else {
    if (mouse_is_in)
      mouse_is_in->enterleave(false, newpos);
    mouse_is_in = 0;
  }
}

bool GBParent::mouse(int button, bool in_not_out, const Point &pos) {
  motion(pos);
  return (mouse_is_in) ? mouse_is_in->mouse(button, in_not_out, pos) : false;
}

bool GBParent::key(int key, bool in_not_out) {
  if (focus_child && focus_child->key(key, in_not_out))
    return true;

  for (list<GBChildInfo>::iterator i = children.begin();
       i != children.end(); ++i)
    if ((*i).child != focus_child && (*i).child->key(key, in_not_out))
      return true;

  return false;
}

GBox *GBParent::whereis(const Point &p) {
  if (!bbox().contains(p))
    return 0;

  for (list<GBChildInfo>::iterator i = children.end();
       i != children.begin(); ) {
    --i;
    if ((*i).child->bbox().contains(p))
      return (*i).child;

    if ((*i).ispopup)
      return 0;
  }
  return 0;
}

list<GBChildInfo>::iterator GBParent::findit(GBox *ch) {
  if (!ch)
    return children.end();

  for (list<GBChildInfo>::iterator i = children.begin();
       i != children.end(); ++i)
    if ((*i).child == ch)
      return i;

  return children.end();
}

void GBParent::placechildren() {
  for (list<GBChildInfo>::iterator i = children.begin();
       i != children.end(); ++i)
    (*i).unplace();
  if (autoresize)
    resize(Area(0, 0));
  placechildren_(GBP_Hori);
  placechildren_(GBP_Vert);
}

void GBParent::placechildren_(bool vnoth) {
  list<GBChildInfo>::iterator i;
  bool firsttime = true;
  bool allplaced = false;
  bool noneplaced = false;
  int lastchainsize;
  while (!allplaced) {
    bool chainplaced = false;
    if (noneplaced || (autoresize && firsttime))
      for (i = children.begin(); i != children.end(); ++i)
        if ((*i).forms_chain(vnoth, this)) {
          lastchainsize = pc_chain(vnoth, *i, firsttime && autoresize);
          chainplaced = true;
          break;
        }

    if (firsttime && autoresize) {
      if (!chainplaced)
        athrow(
          "GBParent::placechildren: autoresizing parent without chain");
      // compute size=size(lastchainsize) ****
    }
    if (noneplaced && !chainplaced)
      athrow("GBParent::placechildren: cannot layout: insufficient info");

    firsttime = false;
    noneplaced = true;
    allplaced = true;
    for (i = children.begin(); i != children.end(); ++i) {
      if (!(*i).placed(vnoth))
        if ((*i).fixed_depend(vnoth, this)) {
          pc_one(vnoth, *i);
          noneplaced = false;
        }
      allplaced &= (*i).placed(vnoth);
    }
  }
}

void GBParent::pc_one(bool vnoth, GBChildInfo &ch) {
  if (ch.double_depend(vnoth)) {
    int min, max;
    if (!pc_get_minmax(vnoth, GBP_Min, ch, min)
        || !pc_get_minmax(vnoth, GBP_Max, ch, max))
      athrow(
        "GBParent::pc_one: Couldn't determine position for a doubly connected child");

    int rest = (max - min) - ch.size(vnoth);
    int mindist = ch.dist(vnoth, GBP_Min);
    int maxdist = ch.dist(vnoth, GBP_Max);
    if (mindist < 0 || maxdist < 0)
      athrow("GBParent::pc_one: Doubly connected child cannot use `Align'");
    if (mindist == 0 && maxdist == 0)
      mindist = maxdist = 1; // _have_ to spread distance somehow.
    int minrest = int(1.0 * rest * mindist / (mindist + maxdist));
    ch.moveto(vnoth, min + minrest);
    ch[vnoth].placed = true;
  } else {
    int mm;
    if (pc_get_minmax(vnoth, GBP_Min, ch, mm))
      ch.moveto(vnoth, mm + max(ch.dist(vnoth, GBP_Min), 0));
    else if (pc_get_minmax(vnoth, GBP_Max, ch, mm))
      ch.moveto(vnoth, mm - ch.size(vnoth) - max(ch.dist(vnoth, GBP_Max), 0));

    else
      athrow(
        "GBParent::pc_one: couldn't determine position for a singly connected child");

  }
}

bool GBParent::pc_get_minmax(bool vnoth, bool side,
                             GBChildInfo &ch, int &res) {
  if (ch.whence(vnoth, side) == this) {
    res = ch.edge(vnoth, side, side);
    return true;
  } else if (ch.edge_fixed(vnoth, side, this)) {
    res = ch.edge(vnoth, side, side ^ (ch.dist(vnoth, side) >= 0));
    return true;
  }
  return false;
}

int GBParent::pc_chain(bool vnoth, GBChildInfo &start, bool freemax) {
  // returns width of chain
  vector<GBChildInfo *> chain;
  vector<int> ddist; // double distance to left of each gbox
  GBChildInfo *el = &start;
  int min = start.edge(vnoth, 0, start.whence(vnoth, 0) != this);
  int tsize = 0; // total of sizes of members
  int tddist = 0; // double total of requested distances between members
  GBox *next = 0;
  bool first = true;
  while (el) {
    chain.push_back(el);
    tsize += el->size(vnoth);
    if (el->dist(vnoth, GBP_Min) < 0 || el->dist(vnoth, GBP_Max) < 0)
      athrow("GBParent::pc_chain: `Align' in chain not allowed");
    if (first) {
      ddist.push_back(2 * el->dist(vnoth, GBP_Min));
      tddist += ddist[0];
    } else {
      int ndi = el->dist(vnoth, GBP_Min);
      ddist[ddist.size() - 1] += ndi;
      tddist += ndi;
    }
    int ndi = el->dist(vnoth, GBP_Max);
    ddist.push_back(ndi);
    tddist += ndi;
    next = el->whence(vnoth, GBP_Max);
    if (next == this)
      el = 0;
    else {
      el = find(next);
      if (el->placed(vnoth))
        el = 0;
    }
    if (!el) {
      ddist[ddist.size() - 1] += ndi;
      tddist += ndi;
    }
    first = false;
  }
  int max = chain[chain.size() - 1]->edge(vnoth, 1, next == this);
  int avail = freemax ? (tsize + tddist / 2) : max;
  int spare = avail - tsize;
  if (tddist == 0) {
    for (unsigned int i = 0; i < chain.size(); i++)
      ddist[i] = 1;
    tddist = chain.size() + 1;
  }
  for (unsigned int i = 0; i < chain.size(); i++) {
    int dist = int(1.0 * spare * ddist[i] / (tddist + .0001));
    // tddist can be zero iff ddist[i]=0. We wish 0/0=0, hence the +.0001.
    chain[i]->moveto(vnoth, min + dist);
    min += dist + chain[i]->size(vnoth);
    spare -= dist;
    tddist -= ddist[i];
  }
  return avail;
}

void GBParent::shift(int dx, int dy) {
  for (list<GBChildInfo>::iterator i = children.begin();
       i != children.end();
       ++i)
    if ((*i).placed() && (*i).child)
      (*i).child->shift(dx, dy);
  GBox::shift(dx, dy);
}