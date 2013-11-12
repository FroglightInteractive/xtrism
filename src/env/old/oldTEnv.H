// TEnv.H - private header file for TEnv - Trisms portable environment

#ifndef _TEnv_H
#define _TEnv_H

#include <string>
#include <vector>
#include <stack>

#include "../basics/dbx.H"

#ifdef TPRIVATE
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#else
#include "TXID.H"
#endif

#include "TCmap.H"
#include "../basics/BBox.H"

class TReso { // dev.indep.
  public: 
    TReso(int xx, int yy, int mminfact, int mmaxfact, bool fs=0):
      x(xx), y(yy), minfact(mminfact), maxfact(mmaxfact), fullscr(fs) {}
    int best(int sw, int sh) const;
    int actx(int f)  const { return x*f; }
    int acty(int f) const { return y*f; }
    bool fullscreen() const { return fullscr; }
  private:
    int x, y, minfact, maxfact;
    bool fullscr;
  };

typedef void (*THFoo)(void *arg, int, int, int, int);
class THandler { // dev.indep.
  public:
    THandler(THFoo f,void *a=0): foo(f),arg(a) {}
    THandler(void(*f)(void *, int, int, int), void *a=0):
      foo(THFoo(f)),arg(a) {}
    THandler(void(*f)(void *, int, int), void *a=0): foo(THFoo(f)),arg(a) {}
    THandler(void(*f)(void *, int), void *a=0): foo(THFoo(f)),arg(a) {}
    THandler(void(*f)(void *), void *a=0): foo(THFoo(f)),arg(a) {}
    THandler(): foo(0), arg(0) {}
    bool operator==(const THandler &other) const
      { return foo==other.foo && arg==other.arg; }
    void operator()(int a=0, int b=0, int c=0, int d=0)
      { if (foo)
          (*foo)(arg,a,b,c,d);
      }
  private:
    THFoo foo;
    void *arg;
  };

class THStack: public stack<vector<THandler> > {
  public:
    void operator()(int a=0, int b=0, int c=0, int d=0)
      { if (!empty())
          { top()(a,b,c,d); dbx(4,"THStack: call; %p",this); }
        else
          { dbx(4,"THStack: no call; %p",this); }
      }
  };

#include "TGFX.H"

class TEnv: public TGFXP {
  public: // dev.indep.
    TEnv(const TReso &reso, 
         int argc, char **argv,
         char *addit=0 /* display name for X */);
    ~TEnv() { delete tcmap; }

    int actualfactor() const { return actfactor; };
    void loop();
    void reg_keyboard(const THandler &h) { doreg(keyboard,h); }
    void dereg_keyboard(const THandler &h) { dodereg(keyboard,h); }
    void reg_mml(const THandler &mou, const THandler &mot,
                 const THandler &lea);
    void dereg_mml(const THandler &mou, const THandler &mot,
                 const THandler &lea);
    void reg_redraw(const THandler &h) { doreg(redraw,h); }
    void dereg_redraw(const THandler &h);
    void reg_quit(const THandler &h) { doreg(quith,h); }
    void dereg_quit(const THandler &h) { dodereg(quith,h); }
    void reg_poll(const THandler &h) { doreg(poll,h); }
    void dereg_poll(const THandler &h) { dodereg(poll,h); }
    void reg_sync(const THandler &h) { doreg(sync,h); }
    void dereg_sync(const THandler &h) { dodereg(sync,h); }
    void quit();
    void confine(); // confine the mouse pointer to our window and hide it
    void release(); // release the pointer
    string id() const;
    Point pointer(bool *inme_ret=0) const;
    bool candosync() const { return false; } // dev. dep.
    int width() const { return wid; }
    int height() const { return hei; }
    bool setautorep(bool on_not_off)
      { bool old=wishautorep; wishautorep=on_not_off; return old; }
    int lastchar();
  private: // dev.indep.
    int wid, hei;
    int actfactor;
    THStack keyboard, mouse, leave, motion, redraw, quith, poll, sync;
    void doreg(THStack &stack, const THandler &h);
    void dodereg(THStack &stack, const THandler &h);
    void call(THStack &stack, int a=0, int b=0, int c=0, int d=0) const;
    bool confined;
    bool containspointer;
    void reenter();
#ifdef TPRIVATE
  public: // dev.dep.
    Visual *vis() const { return visual; }
    int depth() const { return DefaultDepthOfScreen(screen); }
  private: // dev.dep.
    void do_setautorep(bool a);
    bool getautorep();
    void claimrep();
    void releaserep();
#endif
#ifdef TPRIVATE
  private: // dev.dep.
    Screen *screen;
    Visual *visual;
    Atom wmdelw, wmprot;
    bool repclaimed, wishautorep, extautorep;
    XKeyEvent *lastkey;
#else
  private: // dev.dep.
    void *screen;
    void *visual;
    TXID wmdelw, wmprot;
    bool repclaimed, wishautorep, extautorep;
    void *lastkey;
#endif
  };

#endif
