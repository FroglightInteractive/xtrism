// ClickGrp.H

#ifndef _ClickGrp_H
#define _ClickGrp_H

#include "RedrawGrp.H"

/* Derived classes of Clickable should provide a function click() to
   accept mouse clicks.
 * If you are interested in key presses, set canhavefocus=true, and
   provide a function key() to accept key presses.
 * When keyboard focus is transferred to you, takefocus() is called. You
   should graphically emphasize the fact that you have focus, unless you
   are in a sharedfocus group.
 * When keyboard focus is transferred away from you, losefocus() is called.
 * If you wish to take focus, (eg because of a mouse click) use
   group.focusrequest(this) to alert the group. takefocus() will be called
   if it worked. (It usu. does.)
 */

/* Groups can be nested: A clickable can contain another group within it.
   Use makechild() to create the new group, and note that YOU are responsible
   for deleting the child later on.
 * toplevel sharedfocus groups can be made to accept focus from outside:
   useful if the group contains games, (which don't request focus from
   inside).
 */

class Clickable: public Redrawable {
  public:
    Clickable(class ClickGrp &grp, int w, int h,
              const BoxRelPos &hrp, const BoxRelPos &vrp);
    Clickable(class ClickGrp &grp, int x, int y, int w, int h);
    virtual ~Clickable();
    class ClickGrp *makechild(bool shf=0);
    void forgetchild() { child=0; }
  protected:
    virtual void takefocus() { hasfocus=1; }
    virtual void losefocus() { hasfocus=0; }
    virtual void click(int button, bool io, int x, int y) {}
    virtual bool key(int key, bool io) { return 0; }
    // return 1 if you handled the key (othw. it will be passed to siblings
    // if group is sharedfocus).

    void tfocus();
    void mclick(int button, bool io, int x, int y);
    bool kkey(int key, bool io);
    void lfocus();
    void focusrequest(); // from child
    bool canhavefocus;
    bool hasfocus; 
  friend class ClickGrp;
  private:
    class ClickGrp &group;
    class ClickGrp *child;
  };

class ClickGrp {
  public:
    ClickGrp(RedrawGrp &rdgrp, TEnv &tenv, bool shf=0);
    ~ClickGrp();
    void takefocus(); // only works if sharedfocus
    TGFX tgfx() { return rdg.tgfx(); }
  protected:
    ClickGrp(RedrawGrp &rdgrp, Clickable *par, bool shf);
    bool click(int button, bool io, int x, int y);
    bool key(int key, bool io);
    void add(Clickable *clk) { clkl.push_back(clk); }
    void remove(Clickable *clk) { clkl.remove(clk); }
    void focusrequest(Clickable *clk);
    void losefocus();
  friend void clickgrp_click(ClickGrp *, int, int, int, int);
  friend void clickgrp_key(ClickGrp *, int, int);
  friend class Clickable;
  protected:
    RedrawGrp &rdg;
  private:
    list<Clickable *> clkl;
    TEnv *tenvp; // 0 if parented by Clickable
    Clickable *parent; // 0 if not parented
    bool sharedfocus;
  };
#endif
