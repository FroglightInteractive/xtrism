// ByteImage.C

#include "ByteImage.h"
#include "ByteMap.h"
#include "RGBMap.h"
#include "WideRGB.h"

/* ------------------------------- byteimage ------------------------------ */
void byteimage(RGBMap &dst, ByteMap const &src,
               WideRGB const &rgb0, WideRGB const &rgb1) {
  int w = std::min(dst.width(), src.width());
  int h = std::min(dst.height(), src.height());
  if (w * h < 256 + 10) {
    for (int y = 0; y < h; y++)
      for (int x = 0; x < w; x++) {
        double g1 = src.cc(x, y) / 255.;
        double g0 = 1.0 - g1;
        dst.rset(x, y,
                 rgb0.r * g0 + rgb1.r * g1,
                 rgb0.g * g0 + rgb1.g * g1,
                 rgb0.b * g0 + rgb1.b * g1);
      }
  } else {
    RGB *table = new RGB[256];
    for (int grey = 0; grey < 256; grey++) {
      double g1 = grey / 255.;
      double g0 = 1. - g1;
      table[grey] = RGB(int(rgb0.r * g0 + rgb1.r * g1),
                        int(rgb0.g * g0 + rgb1.g * g1),
                        int(rgb0.b * g0 + rgb1.b * g1));
    }
    for (int y = 0; y < h; y++)
      for (int x = 0; x < w; x++)
        dst.c(x, y) = table[src.cc(x, y)];
    delete[] table;
  }
}
