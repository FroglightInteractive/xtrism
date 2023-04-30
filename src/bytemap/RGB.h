// RGB.h

#ifndef RGB_H

#define RGB_H

class RGB {
public:
  RGB(unsigned char r0=0, unsigned char g0=0, unsigned char b0=0):
    r(r0), g(g0), b(b0), a(255) {
  }
  RGB(unsigned char grey): r(grey), g(grey), b(grey), a(255) {
  }
  RGB &operator=(const RGB &oth) {
    r = oth.r;
    g = oth.g;
    b = oth.b;
    return *this;
  }
  RGB &operator+=(const RGB &oth) {
    r += oth.r;
    g += oth.g;
    b += oth.b;
    return *this;
  }
  RGB &operator-=(const RGB &oth) {
    r -= oth.r;
    g -= oth.g;
    b -= oth.b;
    return *this;
  }
  unsigned char r, g, b, a;
};

#endif
