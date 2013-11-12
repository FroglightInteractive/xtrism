// RedrawGrp.H

#ifndef _RedrawGrp_H
#define _RedrawGrp_H

#include <string>
#include <list>
using namespace std;

#include "../env/TEnv.H"
#include "../boxes/box.H"

class Redrawable: public Box, public TGFX {
  public:
    Redrawable(class RedrawGrp &grp, int w, int h,
               const BoxRelPos &hrp, const BoxRelPos &vrp);
    Redrawable(class RedrawGrp &grp, int x, int y, int w, int h);
    virtual ~Redrawable();
  protected:
    class RedrawGrp &group;
    virtual void redraw(const BBox &rect) {}
  friend class RedrawGrp;
  };

class RedrawGrp {
  public:
    RedrawGrp(TEnv &env);
    ~RedrawGrp();
    TGFX tgfx() { return tenv; }
  protected:
    void redraw(const BBox &bbox);
    void add(Redrawable *rdw) { rdwl.push_back(rdw); }
    void remove(Redrawable *rdw) { rdwl.remove(rdw); }
  friend void redrawgrp_redraw(RedrawGrp *, int, int, int, int);
  friend class Redrawable;
  private:
    TEnv &tenv;
    list<Redrawable *> rdwl;
  };

#endif
