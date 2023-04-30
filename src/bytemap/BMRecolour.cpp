// BMRecolour.C
// This code is almost entirely stolen from ../pics/Recolour.C vsn 1.1

#include "BMRecolour.h"
#include "ByteMap.h"
#include "../basics/BBox.h"
#include "../basics/minmax.h"
#include <math.h>

inline void recolour(ByteMap *img,
                     int x, int y,
                     float mul, int add) {
  unsigned char &c(img->c(x, y));
  c = clip255(c * mul + add);
}

inline void recolour(ByteMap *img,
                     int x, int y,
                     int add) {
  unsigned char &c = img->c(x, y);
  c = clip255(c + add);
}

void recolour_rectangle(ByteMap *img, BBox const &bbox,
                        float mul, int add) {
  for (int y = bbox.top(); y < bbox.bottom(); y++)
    for (int x = bbox.left(); x < bbox.right(); x++)
      recolour(img, x, y, mul, add);
}

void recolour_rect_edges(ByteMap *img, BBox const &bbox,
                         int bw,
                         int depth) {
  int x, y;
  int w = bbox.width() + 2 * bw, h = bbox.height() + 2 * bw;
  int x0 = bbox.left() - bw, y0 = bbox.top() - bw;
  int x1 = x0 + w - 1, y1 = y0 + h - 1;
  for (y = 1; y <= bw; y++)
    for (x = 0; x < w - y; x++) {
      recolour(img, x + x0, y + y0 - 1, 1., depth);
      recolour(img, x1 - x, y1 + 1 - y, 1., -depth);
    }
  for (x = 1; x <= bw; x++)
    for (y = bw; y < h - x; y++) {
      recolour(img, x + x0 - 1, y + y0, 1., depth);
      recolour(img, x1 - x + 1, y1 - y, 1., -depth);
    }
}

void recolour_circle(ByteMap *img,
                     QPoint const &centre,
                     int radius, float mul, int add,
                     int r1, int depth) {
  int rad2 = radius * radius;
  int r12 = r1 * r1;
  for (int y = -r1; y <= r1; y++) {
    int y2 = y * y;
    // int bdx=int(sqrt(rad2-y2));
    int bdx1 = int(sqrt(r12 - y2));
    for (int x = -bdx1; x <= bdx1; x++) {
      int x2 = x * x;
      int r2 = x2 + y2;
      if (r2 <= rad2)
        recolour(img, centre.x() + x, centre.y() + y,
                 mul, add);
      else
        recolour(img, centre.x() + x, centre.y() + y,
                 1., -depth * (y + x) / sqrt(r2));
    }
  }
}
