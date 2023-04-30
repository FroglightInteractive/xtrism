// RGBMap.h

#ifndef RGBMAP_H
#define RGBMAP_H

// RGBMap.H

// As ByteMap, but for full colour RGB pictures


#include "../basics/minmax.h"
#include <QImage>
#include "RGB.h"

class RGBMap: public QImage {
public:
  RGBMap(unsigned int w, unsigned int h);
  RGBMap(QString filename);
  ~RGBMap();
  RGBMap(RGBMap const &oth, unsigned int x, unsigned int y,
         unsigned int w, unsigned int h);
  RGBMap(RGBMap const &oth, int rot);   // in units of 90 deg, ccw
  void load(QString filename);
  RGB *data() {
    return dat;
  }
  RGB &c(unsigned int x, unsigned int y) {
    return dat[x + wid * y];
  }
  inline void set(unsigned int x, unsigned int y,
           unsigned char r, unsigned char g, unsigned char b) {
    RGB &a = c(x, y);
    a.r = r;
    a.g = g;
    a.b = b;
  }
  template <class T> void rset(unsigned int x, unsigned int y, T r, T g, T b) {
    set(x, y,
        (r<0) ? 0 : (r>255) ? 255 : (unsigned char)r,
        (g<0) ? 0 : (g>255) ? 255 : (unsigned char)g,
        (b<0) ? 0 : (b>255) ? 255 : (unsigned char)b);
  }
  RGB const &cc(unsigned int x, unsigned int y) const {
    return dat[x + wid * y];
  }
  RGB &sc(unsigned int x, unsigned int y) {
    if (x >= wid || y >= hei)
      throw("RGBMap: illegal access");
    return c(x, y);
  }
  RGB const &scc(unsigned int x, unsigned int y) const {
    if (x >= wid || y >= hei)
      throw("RGBMap: illegal access");
    return cc(x, y);
  }
  RGB *line(unsigned int y) {
    return dat + wid * y;
  }
  RGB const *cline(unsigned int y) const {
    return dat + wid * y;
  }
  RGB *sline(unsigned int y) {
    if (y >= hei)
      throw("RGBMap: illegal access");
    return dat + wid * y;
  }
  RGB const *scline(unsigned int y) const {
    if (y >= hei)
      throw("RGBMap: illegal access");
    return dat + wid * y;
  }

  unsigned int width() const {
    return wid;
  }
  unsigned int height() const {
    return hei;
  }
private:
  // RGBMap(RGBMap const &) = delete;
  // RGBMap &operator=(RGBMap const &) = delete;
protected:
  unsigned int wid;
  unsigned int hei;
  RGB *dat;
};

#endif
