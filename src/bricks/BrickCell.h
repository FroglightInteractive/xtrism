// brickcell.H
// visual definition of a single cell of a Trism brick

/* A BrickCell contains a map of the pixels in a rectangular cell.
 * One byte is used for each pixel, no padding is used between lines.
 * The top and left borders use pixel values 0..127.
 * The bottom and right borders use pixel values 128..255.
 * The central part of the cell uses pixel values 64..191.
 * ! These values are an indication only !
 * A function is provided to draw the borders.
 * Constructors are provided to select a piece of a cell, or to produce
   a rotated copy.

 * This object is not suitable for plotting to a drawable directly: use
   the (device dependent) BrickCellX for that purpose.
 */

#ifndef _brickcell_h
#define _brickcell_h

#include <stdio.h>
#include "GrayImage.h"

class BCSurround {
public:
  BCSurround(bool E=0, bool NE=0, bool N=0, bool NW=0,
             bool W=0, bool SW=0, bool S=0, bool SE=0):
    e(E), ne(NE), n(N), nw(NW),
    w(W), sw(SW), s(S), se(SE) {
  }
  unsigned const int e : 1, ne : 1, n : 1, nw : 1, w : 1, sw : 1, s : 1,
    se : 1;
};

class BrickCell: public GrayImage {
public:
  BrickCell(unsigned int w, unsigned int h=0, unsigned int depth=128);
  BrickCell(GrayImage const &oth): GrayImage(oth) { }
  BrickCell(GrayImage &&oth): GrayImage(oth) { }
  void drawborders(unsigned int borderwidth, const BCSurround &sur,
                   int depth=100);
  void drawstripes(unsigned int depth=50);
private:
  unsigned char pixc(unsigned int x, unsigned int y) {
    return (x < wid && y < hei) ? pix(x, y) : 128;
  }
  inline unsigned int random(unsigned int w, unsigned int n, unsigned int nw);
  void drawbasis(unsigned int depth);

  inline void a_rect(unsigned int x, unsigned int y,
                     unsigned int w, unsigned int h, int o);
  inline void a_tri_ne(unsigned int x, unsigned int y,
                       unsigned int w, int o);
  inline void a_tri_nw(unsigned int x, unsigned int y,
                       unsigned int w, int o);
  inline void a_tri_se(unsigned int x, unsigned int y,
                       unsigned int w, int o);
  inline void a_tri_sw(unsigned int x, unsigned int y,
                       unsigned int w, int o);
  inline void a_line_nw_se(unsigned int x, unsigned int y,
                           unsigned int w, int o);
  inline void a_line_ne_sw(unsigned int x, unsigned int y,
                           unsigned int w, int o);
  // (x,y) = nw corner (!)
};

#endif
