// GrayImage.h

#ifndef GRAYIMAGE_H

#define GRAYIMAGE_H

// GrayImage.H

#ifndef _GrayImage_H
#define _GrayImage_H

#include <QImage>

class GrayImage {
public:
  GrayImage(unsigned int w, unsigned int h); // no initialization
  GrayImage(unsigned int w, unsigned int h, unsigned char ini);
  GrayImage(GrayImage const &);
  GrayImage(GrayImage &&);
  GrayImage &operator=(GrayImage const &);
  GrayImage &operator=(GrayImage &&);
  GrayImage cropped(unsigned int x, unsigned int y,
                    unsigned int w, unsigned int h) const;
  GrayImage rotated(int rot) const;   // in units of 90 deg, ccw
  static GrayImage fromFile(QString filename);
  static GrayImage fromQImage(QImage);
  QImage const &toQImage() const;
  void save(QString filename);
public:  
  unsigned char *data() {
    return dat;
  }
  void set(unsigned int x, unsigned int y, unsigned char gray) {
    dat[x + bpl*y] = gray;
  }
  template <class T> void rset(unsigned int x, unsigned int y, T gray) {
    set(x, y, (gray<0) ? 0 : (gray>255) ? 255 : (unsigned char)gray);
  }
  unsigned char get(unsigned int x, unsigned int y) const {
    return dat[x + bpl*y];
  }
  unsigned char &pix(unsigned int x, unsigned int y) {
    return dat[x + bpl*y];
  }
  unsigned char const *line(unsigned int y) const {
    return dat + bpl*y;
  }
  unsigned char *line(unsigned int y) {
    return dat + bpl*y;
  }
  unsigned int width() const {
    return wid;
  }
  unsigned int height() const {
    return hei;
  }
  void alter(int x, int y, int o) {
    alter(pix(x, y), o);
  }
public:
  static void sub(unsigned char &p, unsigned int o) {
    p = (o<p)? p-o : 0;
  }
  static void add(unsigned char &p, unsigned int o) {
    p = (p+o<255) ? p+o : 255;
  }
  static void alter(unsigned char &p, int o) {
    if (o<0)
      sub(p, -o);
    else if (o>0)
      add(p, o);
  }
protected:
  unsigned char *dat;
  unsigned int wid;
  unsigned int hei;
  unsigned int bpl;
  QImage store;
};

#endif
class GrayImage {
public:
  GrayImage();
};

#endif
