// TDrawCol.H - draw in a given color

#ifndef _TDrawCol_H
#define _TDrawCol_H

#include "TGFX.H"
#include "../basics/BBox.H"

#ifndef TPRIVATE
#include "TXID.H"
#endif

class TDrawCol {
  public:
    TDrawCol(const TGFX &tgfx, const TRGB &fg);
    TDrawCol(const TGFX &tgfx, TCol fg);
    ~TDrawCol();
    void point(int x0, int y0) const;
    void line(int x0, int y0, int x1, int y1) const; // } the final point
    void rline(int x0, int y0, int dx, int dy) const; //} IS plotted
    void rectangle(int x0, int y0, int w, int h, bool fill=true) const;
    // fills or outlines { x,y | x0<=x<x0+w & y0<=y<y0+h }
    void rectangle(BBox const &bb, bool fill=true) const
      { rectangle(bb.left(),bb.top(), bb.width(), bb.height(),fill); }
    void ellipse(int x0, int y0, int w, int h, bool fill=true) const;
    // (x0,y0) is the CENTRE of the ellipse
    void circle(int x0, int y0, int r, bool fill=true) const
      { ellipse(x0,y0,r,r,fill); }
    void setclip(BBox const &bb);
  private:
    TGFX const &tgfx;
  private: // device dependent
#ifdef TPRIVATE
    GC gc;
#else
    void *gc;
#endif
  };

#endif
