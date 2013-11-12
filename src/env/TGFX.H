// TGFX.H


#ifndef _TGFX_H
#define _TGFX_H

// baseclass for most other graphics objects s/a fonts, sprites

#ifndef TPRIVATE
#include "TXID.H"
#endif

#include "../basics/Throw.H"
#include "TCmap.H"

const int TGFX_MAGIC=13254707;

class TGFX {
  public:
    TGFX(int magic) { if (magic!=TGFX_MAGIC) athrow("TGFX: Bad magic"); }
    TCol rgb(const TRGB &rgb) const { return (*tcmap)(rgb); }
    TCol grey(int grey) const { return (*tcmap)(TRGB(grey)); }
    const TCmap &cmap() const { return *tcmap; }
  private: // dev.indep.
    TCmap *tcmap;
  friend class TEnv;
#ifdef TPRIVATE
  public: // dev.dep.
    Display *disp() const { return display; }
    Drawable win() const { return drwb; }
  private: // dev.dep.
    Display *display;
    Drawable drwb;
#else
  private:
    void *display;
    TXID drwb;
#endif
  };

#endif
