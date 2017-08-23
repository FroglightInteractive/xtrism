// Recolour.C

#include "Recolour.h"
#include "../env/TEnv.h"
#include "../env/TImage.h"
#include "../basics/BBox.h"
#include <math.h>

inline void recolour(TImage *dst, TImage const *src,
                     int dst_x, int dst_y, int src_x, int src_y,
                     float mul, int add) {
  TCmap const &tcm(src->tenv().cmap());
  TRGB rgb(tcm(src->getpix(src_x, src_y)));
  rgb.recolour(mul, add);
  dst->putpix(dst_x, dst_y, tcm(rgb));
}

inline void recolour(TImage *dst, TImage const *src,
                     int dst_x, int dst_y, int src_x, int src_y,
                     int add) {
  TCmap const &tcm(src->tenv().cmap());
  TRGB rgb(tcm(src->getpix(src_x, src_y)));
  rgb += add;
  dst->putpix(dst_x, dst_y, tcm(rgb));
}

inline void recolour(TImage *img,
                     int x, int y,
                     float mul, int add) {
  TCmap const &tcm(img->tenv().cmap());
  TRGB rgb(tcm(img->getpix(x, y)));
  rgb.recolour(mul, add);
  img->putpix(x, y, tcm(rgb));
}

inline void recolour(TImage *img,
                     int x, int y,
                     int add) {
  TCmap const &tcm(img->tenv().cmap());
  TRGB rgb(tcm(img->getpix(x, y)));
  rgb += add;
  img->putpix(x, y, tcm(rgb));
}

void recolour_rectangle(TImage *img, BBox const &bbox,
                        float mul, int add) {
  for (int y = bbox.top(); y < bbox.bottom(); y++)
    for (int x = bbox.left(); x < bbox.right(); x++)
      recolour(img, x, y, mul, add);
}

void recolour_rect_edges(TImage *img, BBox const &bbox,
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

void recolour_circle(TImage *img,
                     Point const &centre,
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

void recolour_topline(TImage *dst,
                      TImage const *src, Point const &srctopleft,
                      int depth, bool /*left*/, bool right) {
  int bw = dst->height();
  int w = dst->width();
  for (int y = 1; y <= bw; y++)
    for (int x = w - 1 - right ? y : 0; x >= 0; x--)
      recolour(dst, src, x, y - 1,
               x + srctopleft.x(), y + srctopleft.y() - 1,
               depth);
}

void recolour_bottomline(TImage *dst,
                         TImage const *src,
                         Point const &srctopleft,
                         int depth, bool left, bool /*right*/) {
  int bw = dst->height();
  int w = dst->width();
  for (int y = 1; y <= bw; y++)
    for (int x = left ? y : 0; x < w; x++)
      recolour(dst, src, x, bw - y,
               x + srctopleft.x(), srctopleft.y() + bw - y,
               -depth);
}

void recolour_leftline(TImage *dst,
                       TImage const *src,
                       Point const &srctopleft,
                       int depth) {
  int bw = dst->width();
  int h = dst->height();
  for (int x = 0; x < bw; x++)
    for (int y = 0; y < h; y++)
      recolour(dst, src, x, y, x + srctopleft.x(), y + srctopleft.y(), depth);

}

void recolour_rightline(TImage *dst,
                        TImage const *src,
                        Point const &srctopleft,
                        int depth) {
  int bw = dst->width();
  int h = dst->height();
  for (int x = 0; x < bw; x++)
    for (int y = 0; y < h; y++)
      recolour(dst, src, x, y, x + srctopleft.x(), y + srctopleft.y(),
               -depth);
}