// RGBImage.H

#ifndef _RGBImage_H
#define _RGBImage_H

#include <QImage>
#include "WideRGB.h"
#include <QRectF>

class RGBImage {
public:
  RGBImage(unsigned int w, unsigned int h); // no initialization
  RGBImage(unsigned int w, unsigned int h, uint ini);
  RGBImage(RGBImage const &);
  RGBImage(RGBImage &&);
  RGBImage &operator=(RGBImage const &);
  RGBImage &operator=(RGBImage &&);
  RGBImage cropped(unsigned int x, unsigned int y,
                    unsigned int w, unsigned int h) const;
  RGBImage rotated(int rot) const;   // in units of 90 deg, ccw
  static RGBImage fromFile(QString filename);
  static RGBImage fromQImage(QImage const &);
  static RGBImage colorized(class GrayImage const &,
                            WideRGB const &rgb0, WideRGB const &rgb1);
  /* Copies a GrayImage into an RGB image, using RGB0 and RGB1
     as a linear map:

                 v |-> rgb0 + (rgb1-rgb0)*(v/255)

     rgb0 and rgb1 do not have to lie within [0,255]^3: the
     rgb values are clipped automatically.
 */
  static RGBImage colorized(GrayImage const &, QRectF rainbowSource);
  static RGBImage colorized(QSize size, int bw, QRectF rainbowSource);
  QImage const &toQImage() const;
  void save(QString filename) const;
public:  
  uint *data() {
    return dat;
  }
  void set(unsigned int x, unsigned int y, uint rgb) {
    dat[x + wpl*y] = rgb;
  }
  template <class T> void rset(unsigned int x, unsigned int y,
                               T r, T g, T b) {
    dat[x + wpl*y] = qRgb(clip255(r), clip255(g), clip255(b));
  }
  uint get(unsigned int x, unsigned int y) const {
    return dat[x + wpl*y];
  }
  uint &pix(unsigned int x, unsigned int y) {
    return dat[x + wpl*y];
  }
  uint const *line(unsigned int y) const {
    return dat + wpl*y;
  }
  uint *line(unsigned int y) {
    return dat + wpl*y;
  }
  unsigned int width() const {
    return wid;
  }
  unsigned int height() const {
    return hei;
  }
  QSize size() const {
    return QSize(wid, hei);
  }
protected:
  uint *dat;
  unsigned int wid;
  unsigned int hei;
  unsigned int wpl;
  QImage store;
};

#endif
