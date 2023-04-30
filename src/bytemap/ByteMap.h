// ByteMap.H

#ifndef _ByteMap_H
#define _ByteMap_H

#include <QImage>

class ByteMap {
public:
  ByteMap(unsigned int w, unsigned int h, unsigned char ini=0);
  ByteMap(QString filename);
  ~ByteMap();

  ByteMap(ByteMap const &oth, unsigned int x, unsigned int y,
          unsigned int w, unsigned int h);
  ByteMap(ByteMap const &oth, int rot);   // in units of 90 deg, ccw
  void write(QString filename) const;

  unsigned char *data() {
    return dat;
  }
  void set(unsigned int x, unsigned int y, unsigned char gray) {
    dat[x + wid*y] = gray;
  }
  template <class T> void rset(unsigned int x, unsigned int y,
                               T gray) {
    set(x, y, (gray<0) ? 0 : (gray>255) ? 255 : (unsigned char)gray);
  }
  unsigned char &c(unsigned int x, unsigned int y) {
    return dat[x + wid * y];
  }
  unsigned char cc(unsigned int x, unsigned int y) const {
    return dat[x + wid * y];
  }
  unsigned char &sc(unsigned int x, unsigned int y) {
    if (x >= wid || y >= hei)
      throw("ByteMap: illegal access");
    return c(x, y);
  }
  unsigned char scc(unsigned int x, unsigned int y) const {
    if (x >= wid || y >= hei)
      throw("ByteMap: illegal access");
    return cc(x, y);
  }
  unsigned char *line(unsigned int y) {
    return dat + wid * y;
  }
  unsigned char const *cline(unsigned int y) const {
    return dat + wid * y;
  }
  unsigned char *sline(unsigned int y) {
    if (y >= hei)
      throw("ByteMap: illegal access");
    return dat + wid * y;
  }
  unsigned char const *scline(unsigned int y) const {
    if (y >= hei)
      throw("ByteMap: illegal access");
    return dat + wid * y;
  }

  unsigned int width() const {
    return wid;
  }
  unsigned int height() const {
    return hei;
  }

  void sub(unsigned char &p, unsigned int o) {
    p = std::max(p - o, 0u);
  }
  void add(unsigned char &p, unsigned int o) {
    p = std::min(p + o, 255u);
  }
  void alter(unsigned char &p, int o) {
    p = std::max(std::min(p + o, 255), 0);
  }
  void alter(int x, int y, int o) {
    alter(c(x, y), o);
  }
private:
  // ByteMap(ByteMap const &) = delete;
  // ByteMap &operator=(ByteMap const &) = delete;
protected:
  unsigned int wid;
  unsigned int hei;
  unsigned char *dat;
  QVector<unsigned char> store;
  friend class BrickCell;
};

#endif
