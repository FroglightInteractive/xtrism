// ByteMap.H

#ifndef _ByteMap_H
#define _ByteMap_H

#include <string>
#include "../basics/Byte.H"
#include "../basics/Throw.H"
#include "../basics/minmax.H"

class ByteMap {
public:
  ByteMap(unsigned int w, unsigned int h, byte ini=0);
  ByteMap(unsigned int w, unsigned int h, class TDecompress &src);
  ~ByteMap();

  ByteMap(ByteMap const &oth, unsigned int x, unsigned int y,
          unsigned int w, unsigned int h);
  ByteMap(ByteMap const &oth, int rot);   // in units of 90 deg, ccw
  void write(class TCompress &dst) const;

  byte *data() {
    return dat;
  }
  byte &c(unsigned int x, unsigned int y) {
    return dat[x + wid * y];
  }
  byte cc(unsigned int x, unsigned int y) const {
    return dat[x + wid * y];
  }
  byte &sc(unsigned int x, unsigned int y) {
    if (x >= wid || y >= hei)
      athrow("ByteMap: illegal access");
    return c(x, y);
  }
  byte scc(unsigned int x, unsigned int y) const {
    if (x >= wid || y >= hei)
      athrow("ByteMap: illegal access");
    return cc(x, y);
  }
  byte *line(unsigned int y) {
    return dat + wid * y;
  }
  byte const *cline(unsigned int y) const {
    return dat + wid * y;
  }
  byte *sline(unsigned int y) {
    if (y >= hei)
      athrow("ByteMap: illegal access");
    return dat + wid * y;
  }
  byte const *scline(unsigned int y) const {
    if (y >= hei)
      athrow("ByteMap: illegal access");
    return dat + wid * y;
  }

  unsigned int width() const {
    return wid;
  }
  unsigned int height() const {
    return hei;
  }

  void sub(byte &p, unsigned int o) {
    p = max(p - o, 0u);
  }
  void add(byte &p, unsigned int o) {
    p = min(p + o, 255u);
  }
  void alter(byte &p, int o) {
    p = max(min(p + o, 255), 0);
  }
  void alter(int x, int y, int o) {
    alter(c(x, y), o);
  }
protected:
  unsigned int wid;
  unsigned int hei;
  byte *dat;
  friend class BrickCell;
};

#endif