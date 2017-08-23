// ByteImage.C

#include "ByteImage.H"
#include "ByteMap.H"
#include "WideRGB.H"
#include "../env/TImage.H"
#include "../env/TEnv.H"

/* ------------------------------- byteimage ------------------------------ */
void byteimage(TImage &dst, ByteMap const &src,
               WideRGB const &rgb0, WideRGB const &rgb1) {
  int w = min(dst.width(), src.width());
  int h = min(dst.height(), src.height());
  if (w * h < 256 + 10) {
    for (int y = 0; y < h; y++)
      for (int x = 0; x < w; x++) {
        double g1 = src.cc(x, y) / 255.;
        double g0 = 1. - g1;
        dst.putpix(x, y,
                   dst.tenv().rgb(TRGB(int(rgb0.r * g0 + rgb1.r * g1),
                                       int(rgb0.g * g0 + rgb1.g * g1),
                                       int(rgb0.b * g0 + rgb1.b * g1))));
      }
  } else {
    TCol *table = new TCol[256];
    for (int grey = 0; grey < 256; grey++) {
      double g1 = grey / 255.;
      double g0 = 1. - g1;
      table[grey] = dst.tenv().rgb(TRGB(int(rgb0.r * g0 + rgb1.r * g1),
                                        int(rgb0.g * g0 + rgb1.g * g1),
                                        int(rgb0.b * g0 + rgb1.b * g1)));
    }
    for (int y = 0; y < h; y++)
      for (int x = 0; x < w; x++)
        dst.putpix(x, y, table[src.cc(x, y)]);
    delete[] table;
  }
}