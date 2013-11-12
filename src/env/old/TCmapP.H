// TCmapP.H

#ifndef _TCmapP_H
#define _TCmapP_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

class tRGBmap {
  public:
    tRGBmap() {}
    void init(int nr, int ng, int nb, int bbase, Colormap cm, Display *d);
    void init(int b, int rn, int rm, int gn, int gm, int bn, int bm) {
      base=b; rmax=rn-1; rmul=rm; gmax=gn-1; gmul=gm;
      bmax=bn-1; bmul=bm; }
    TCol rgb(const TRGB &rgb) const;
  protected:
    int base;
    int rmax, rmul;
    int gmax, gmul;
    int bmax, bmul;
  friend class TCmap;
  };

class tGreymap {
  public:
    tGreymap() {}
    void init(int n, int bbase,
              Colormap cm,Display *d);
    void init(int b, int n, int m) { base=b; max=n-1; mul=m; }
    TCol grey(int grey) const;
  protected:
    int base;
    int max,mul;
  friend class TCmap;
  };

class TCmap {
  public:
    TCmap(): cmap(0), disp(0) {}
    Visual *init(Display *d);
    TCol rgb(const TRGB &rgb) const { return rgbmap.rgb(rgb); }
    TCol grey(int grey) const { return greymap.grey(grey); }
  private:
    Colormap cmap;
    Display *disp;
    tRGBmap rgbmap;
    tGreymap greymap;
  protected:
    const Colormap &colormap() const { return cmap; }
  friend class TEnv;
  };

#endif
