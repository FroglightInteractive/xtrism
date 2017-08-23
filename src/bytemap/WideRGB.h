// WideRGB.H

#include "../env/TCmap.H"

class WideRGB {
public:
  WideRGB(short rr=0, short gg=0, short bb=0): r(rr), g(gg), b(bb) {
  }
  WideRGB(TRGB const &rgb): r(rgb.r), g(rgb.g), b(rgb.b) {
  }
  operator TRGB() const {
    return TRGB(r, g, b);
  }
  WideRGB &operator+=(int grey) {
    r += grey;
    g += grey;
    b += grey;
    return *this;
  }
  WideRGB &operator-=(int grey) {
    r -= grey;
    g -= grey;
    b -= grey;
    return *this;
  }
public:
  short r, g, b;
};