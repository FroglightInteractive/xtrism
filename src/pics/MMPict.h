// MMPict.H - the infamous cloudscape

#ifndef _MMPict_H
#define _MMPict_H


class MMPict {
public:
  MMPict(int w0, int h0, class MidPtDisp const *tb_cloud0=0,
           class RGBMap *rgbmap0=0, bool doitnow=true);
  class RGBMap const *rgbmap() const {
    return rgbm;
  }
  ~MMPict();
  void drawit(int top, int bottom);
private:
  int wid, hei;
  class MidPtDisp const *tb_cloud;
  class RGBMap *rgbm;
  bool rgbismine, cloudismine;
};

#endif
