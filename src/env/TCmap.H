// TCmap.H

#ifndef _TCmap_H
#define _TCmap_H

#include <string>
using namespace std;

#include "../basics/minmax.H"

#ifdef TPRIVATE
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#else
#include "TXID.H"
#endif

typedef int TCol;

class TRGB { // dev.indep.
  public:
    TRGB(int rr, int gg, int bb): r(rr), g(gg), b(bb) { limit(); }
    TRGB(int grey): r(grey), g(grey), b(grey) { limit(); }
    TRGB(): r(0), g(0), b(0) {}
    TRGB(const char *str) { fill(str); }
    TRGB(const string &str) { fill(str.c_str()); }
    TRGB &operator=(const TRGB &rgb)
      { r=rgb.r; g=rgb.g; b=rgb.b;
        return *this;
      }
    TRGB &operator+=(const TRGB &rgb)
      { limitadd(rgb.r,rgb.g,rgb.b);
        return *this;
      }
    TRGB &operator-=(const TRGB &rgb)
      { limitadd(-rgb.r,-rgb.g,-rgb.b);
        return *this;
      }
    TRGB &operator+=(int grey)
      { limitadd(grey,grey,grey);
        return *this;
      }
    TRGB &operator-=(int grey)
      { limitadd(-grey,-grey,-grey);
        return *this;
      }
    TRGB &recolour(float mul, int add)
      { r=short(r*mul)+add; g=short(g*mul)+add; b=short(b*mul)+add; limit();
        return *this;
      }
    bool operator==(const TRGB &rgb) const
      { return r==rgb.r && g==rgb.g && b==rgb.b; }
    short r, g, b;
  private:
    void fill(const char *str);
    void limit() {
      short max=::max(r,::max(g,b));
      if (max>255)
        { float a=float(255)/max; r=short(r*a); g=short(g*a); b=short(b*a); }
      makeatleast(r,short(0));
      makeatleast(g,short(0));
      makeatleast(b,short(0));
      }
    void limitadd(short rr, short gg, short bb) {
      r+=rr; g+=gg; b+=bb; limit(); }
/*      short max=(r+=rr)>?(g+=gg)>?(b+=bb);
      short min=r<?g<?b;
      if (max<256 && min>=0)
        return;
      int i=4; // hack to avoid hangup...
      while (max>255)
        { short tot=(max==r)?rr:(max==g)?gg:bb;
          float a=float(max-255)/tot;
          r-=short(a*rr); g-=short(a*gg); b-=short(a*bb);
          max=r>?g>?b;
          if (!--i)
            { limit(); return; }
        }
      i=4;
      while (min<0)
        { short tot=(min==r)?rr:(min==g)?gg:bb;
          float a=float(min-0)/tot;
          r-=short(a*rr); g-=short(a*gg); b-=short(a*bb);
          min=r<?g<?b;
          if (!--i)
            { limit(); return; }
        }
        } */
  };

class TCmap {
  public: // dev.indep.
    ~TCmap();
    TCol operator()(const TRGB &rgb) const
      { return (table)
          ?table[((rgb.r&0xf0) >> 4)+
            ((rgb.g&0xf0))+
            ((rgb.b&0xf0) << 4)]
          :(((rgb.r>>rright)<<rleft)+
            ((rgb.g>>gright)<<gleft)+
            ((rgb.b>>bright)<<bleft));
      }
    TRGB operator()(const TCol tcol) const
      { return (palette)
          ?palette[tcol]
          :TRGB((((tcol&rmask)>>rleft)<<rright)+radd,
                (((tcol&gmask)>>gleft)<<gright)+gadd,
                (((tcol&bmask)>>bleft)<<bright)+badd);
      }
    TCol lighter(TCol tcol, int amount=32)
      { TRGB rgb(operator()(tcol)); rgb+=amount;
        return operator()(rgb);
      }
    string id() const;
    unsigned int avecoldep() const { return _avecoldep; }
  private: // dev.dep.
#ifdef TPRIVATE
    Display *disp;
    Colormap cmap;
    bool isprivate;
    Visual *vis;
#else
    void *disp;
    TXID cmap;
    bool isprivate;
    void *vis;
#endif
  private: // dev.indep.
    int rright,rleft, gright,gleft, bright,bleft;
    int rmask, gmask, bmask;
    int radd, gadd, badd;
    int greybits; // 0 unless table
    TRGB *palette;
    TCol *table;
    unsigned int _avecoldep;
  protected: // dev.dep.
#ifdef TPRIVATE
    TCmap(Display *d);
    const Colormap &colormap() const { return cmap; }
    Visual *visual() { return vis; }
#endif
  friend class TEnv;
  private:
    TCmap(); // DON'T USE
    void dynamicinit(int rbits, int gbits, int bbits, int leftbits=0,
                     bool tabulate=false);
    void dynamicinit(int greybits);
    void staticinit(int rpos, int rbits, int gpos, int gbits,
                    int bpos, int bbits, int leftbits=0);
    void findvisual();
    inline TCol nearest(int r,int g,int b, int ngrey, int nr, int ng, int nb);
  };

#endif
