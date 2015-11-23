// TEnv.H - header file for TEnv - Trisms portable environment

#ifndef _TEnv_H
#define _TEnv_H

#include <string>

#include "../basics/Throw.H"
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

#include "THandlers.H"
#include "TCallBack.H"
#include "TGFX.H"

class TEnv: public TGFX {
  public: // dev.indep.
    TEnv(const class TReso &reso, 
         int argc, char **argv,
         char const *addit=0 /* display name for X */);
    ~TEnv();

    void hideCursor();
    void restoreCursor();
    int actualfactor() const { return actfactor; };
    void loop(int nice=0);
    void reg_keyboard(THKey *h) { keyboard.push(h); }
    void dereg_keyboard(THKey *h) { keyboard.pop(h); }
    void reg_mml(THMML *h);
    void dereg_mml(THMML *h);
    void reg_redraw(THRedraw *h) { redraw.push(h); }
    void dereg_redraw(THRedraw *h);
    void reg_quit(THQuit *h) { quith.push(h); }
    void dereg_quit(THQuit *h) { quith.pop(h); }
    void reg_poll(THPoll *h) { poll.push(h); }
    void dereg_poll(THPoll *h) { poll.pop(h); }
    void reg_sync(THSync *h) { sync.push(h); }
    void dereg_sync(THSync *h) { sync.pop(h); }
    void reg_callback(TCallBack *cb, int code=0) { cbl.reg(cb,code); }
    void dereg_callback(TCallBack *cb) { cbl.dereg(cb); }
    void reg_splayer(class SPlayer *sp);
    void dereg_splayer(class SPlayer *sp);
    void quit();
    void confine(); // confine the mouse pointer to our window and hide it
    void release(); // release the pointer
    string id() const;
    Point pointer(bool *inme_ret=0) const;
    bool candosync() const { return false; } // dev. dep.
    unsigned int width() const { return wid; }
    unsigned int height() const { return hei; }
    bool setautorep(bool on_not_off)
      { bool old=wishautorep; wishautorep=on_not_off; return old; }
    int lastchar();
  private: // dev.indep.
#ifdef TPRIVATE
    Window window;
#else
    TXID window;
#endif
    int wid, hei;
    int actfactor;
    THStack<THMML> mml;
    THStack<THKey> keyboard;
    THStack<THRedraw> redraw;
    THStack<THQuit> quith;
    THStack<THPoll> poll;
    THStack<THSync> sync;
    TCBList cbl;
    bool quitting;
    bool confined;
    bool containspointer;
    void reenter();
    int buttonmask(int button);
    int button(int button);
#ifdef TPRIVATE
  public: // dev.dep.
    Visual *vis() const { return visual; }
    unsigned int depth() const { return DefaultDepthOfScreen(screen); }
    unsigned int bpp() const { return _bpp; }
    unsigned int scanlpad() const { return _scanlpad; }
    GC defaultgc() const { return DefaultGCOfScreen(screen); }
  private: // dev.dep.
    void do_setautorep(bool a);
    bool getautorep();
    void claimrep();
    void releaserep();
    void getpixmapformat();
#endif
  private: // dev.indep.
    class SPlayer *spl;
#ifdef TPRIVATE
  public:
    Atom getwmdelw() { return wmdelw; }
    Atom getwmprot() { return wmprot; }
  private: // dev.dep.
    Screen *screen;
    Visual *visual;
    Atom wmdelw, wmprot;
    bool repclaimed, wishautorep, extautorep;
    XKeyEvent *lastkey;
    int _bpp, _scanlpad;
#else
  private: // dev.dep.
    void *screen;
    void *visual;
    TXID wmdelw, wmprot;
    bool repclaimed, wishautorep, extautorep;
    void *lastkey;
    int _bpp, _scanlpad;
#endif
  };

#endif
