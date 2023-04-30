// WideRGB.h

#ifndef WIDERGB_H

#define WIDERGB_H

#include "../basics/minmax.h"
#include "RGB.h"

class WideRGB { 
public:
  WideRGB(RGB rgb): r(rgb.r), g(rgb.g), b(rgb.b) {
  }
  WideRGB(int rr, int gg, int bb): r(rr), g(gg), b(bb) {
    limit();
  }
  WideRGB(int grey): r(grey), g(grey), b(grey) {
    limit();
  }
  WideRGB(): r(0), g(0), b(0) {
  }
  WideRGB(const WideRGB &x): r(x.r), g(x.g), b(x.b) {
  }
  RGB toRGB() const {
    return RGB(r, g, b);
  }
  WideRGB &operator=(const RGB &rgb) {
    r = rgb.r;
    g = rgb.g;
    b = rgb.b;
    return *this;
  }
  WideRGB &operator=(const WideRGB &rgb) {
    r = rgb.r;
    g = rgb.g;
    b = rgb.b;
    return *this;
  }
  WideRGB &operator+=(const WideRGB &rgb) {
    limitadd(rgb.r, rgb.g, rgb.b);
    return *this;
  }
  WideRGB &operator-=(const WideRGB &rgb) {
    limitadd(-rgb.r, -rgb.g, -rgb.b);
    return *this;
  }
  WideRGB &operator+=(int grey) {
    limitadd(grey, grey, grey);
    return *this;
  }
  WideRGB &operator-=(int grey) {
    limitadd(-grey, -grey, -grey);
    return *this;
  }
  WideRGB &recolour(float mul, int add) {
    r = short(r * mul) + add;
    g = short(g * mul) + add;
    b = short(b * mul) + add;
    limit();
    return *this;
  }
  bool operator==(const WideRGB &rgb) const {
    return r == rgb.r && g == rgb.g && b == rgb.b;
  }
  short r, g, b;
private:
  inline void limit() {
    short max = std::max(r, std::max(g, b));
    if (max > 255) {
      float a = float(255) / max;
      r = short(r * a);
      g = short(g * a);
      b = short(b * a);
    }
    makeatleast(r, short(0));
    makeatleast(g, short(0));
    makeatleast(b, short(0));
  }
  void limitadd(short rr, short gg, short bb) {
    r += rr;
    g += gg;
    b += bb;
    limit();
  }
};

#endif
