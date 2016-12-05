// TB24bit.H

#ifndef _TB24bit_H
#define _TB24bit_H

#include "../basics/Byte.H"
#include "../basics/Throw.H"

class TB24bit {
  public:
    TB24bit(unsigned int w, unsigned int h);
    ~TB24bit();
    byte &r(unsigned int x, unsigned int y)
      { illacc(x,y); return data[(x+wid*y)*3]; }
    byte &g(unsigned int x, unsigned int y)
      { illacc(x,y); return data[(x+wid*y)*3+1]; }
    byte &b(unsigned int x, unsigned int y)
      { illacc(x,y); return data[(x+wid*y)*3+2]; }
    void setrgb(unsigned int x, unsigned int y, byte r, byte g, byte b)
      { illacc(x,y); byte *d=data+(x+wid*y)*3;
        d[0]=r; d[1]=g; d[2]=b;
      }
    void fill(class TImage &tim, class BBox const &bb);
  private:
    void illacc(unsigned int x, unsigned int y)
      { tthrow(x>=wid || y>=hei, "TB24bit: illegal access"); }
  private:
    unsigned int wid, hei;
    byte *data;
  };
#endif
