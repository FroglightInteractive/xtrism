// RGBImage.H

#ifndef _RGBImage_H
#define _RGBImage_H

extern void rgbimage(class TImage &dst, class RGBMap const &src);
extern void rgbimage(class TImage &dst, class RGBMap const &src,
                         class BBox const &bb);

class RGBDither {
public:
  RGBDither(class TImage &dst, class RGBMap const &src, unsigned int noise);
  ~RGBDither();
  void dither();
  void dither(int y_top, int y_bot);
private:
  class WideRGB *errs;
  int y_last;
  class TImage &dst;
  class RGBMap const & src;
  unsigned int noise;
};

#endif