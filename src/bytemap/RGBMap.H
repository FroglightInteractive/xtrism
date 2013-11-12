// RGBMap.H

// As ByteMap, but for full colour RGB pictures

#include <stdio.h>
#include "../basics/Byte.H"
#include "../basics/minmax.H"
#include <string>
#include "../basics/Throw.H"

class RGB {
  public:
    RGB(byte r0=0, byte g0=0, byte b0=0): r(r0), g(g0), b(b0) {}
    RGB(byte grey): r(grey), g(grey), b(grey) {}
    RGB &operator=(const RGB &oth) { r=oth.r; g=oth.g; b=oth.b;
                                     return *this; }
    RGB &operator+=(const RGB &oth) { r+=oth.r; g+=oth.g; b+=oth.b;
                                      return *this; }
    RGB &operator-=(const RGB &oth) { r-=oth.r; g-=oth.g; b-=oth.b;
                                      return *this; }
    byte r,g,b;
  };

class RGBMap {
  public:
    RGBMap(unsigned int w, unsigned int h);
    RGBMap(unsigned int w, unsigned int h, FILE *src);
    ~RGBMap();

    RGBMap(RGBMap const &oth, unsigned int x, unsigned int y,
            unsigned int w, unsigned int h);
    RGBMap(RGBMap const &oth, int rot); // in units of 90 deg, ccw
    void write(FILE *dst) const;
    
    RGB *data() { return dat; }
    RGB &c(unsigned int x, unsigned int y) { return dat[x+wid*y]; }
    void set(unsigned int x, unsigned int y, byte r, byte g, byte b)
      { RGB &a=c(x,y); a.r=r; a.g=g; a.b=b; }
    void rset(unsigned int x, unsigned int y, short r, short g, short b)
      { set(x,y,min(max(r,short(0)),short(255)),
	    min(max(g,short(0)),short(255)),
	    min(max(b,short(0)),short(255)));
      }
    RGB const &cc(unsigned int x, unsigned int y) const {return dat[x+wid*y];}
    RGB &sc(unsigned int x, unsigned int y)
      { if (x>=wid || y>=hei)
          athrow("RGBMap: illegal access");
        return c(x,y);
      }
    RGB const &scc(unsigned int x, unsigned int y) const
      { if (x>=wid || y>=hei)
          athrow("RGBMap: illegal access");
        return cc(x,y);
      }
    RGB *line(unsigned int y) { return dat+wid*y; }
    RGB const *cline(unsigned int y) const { return dat+wid*y; }
    RGB *sline(unsigned int y)
      { if (y>=hei)
          athrow("RGBMap: illegal access");
        return dat+wid*y;
      }
    RGB const *scline(unsigned int y) const
      { if (y>=hei)
          athrow("ByteMap: illegal access");
        return dat+wid*y;
      }

    unsigned int width() const { return wid; }
    unsigned int height() const { return hei; }

  protected:
    unsigned int wid;
    unsigned int hei;
    RGB *dat;
  };
