// RGBRecolor.C

#include "RGBRecolor.h"
#include "WideRGB.h"
#include <math.h>

inline void recolor(RGBImage *dst, RGBImage const *src,
                     int dst_x, int dst_y, int src_x, int src_y,
                     float mul, int add) {
  WideRGB rgb(src->get(src_x, src_y));
  rgb.recolor(mul, add);
  dst->pix(dst_x, dst_y) = rgb.toRGB();
}

inline void recolor(RGBImage *dst, RGBImage const *src,
                     int dst_x, int dst_y, int src_x, int src_y,
                     int add) {
  WideRGB rgb(src->get(src_x, src_y));
  rgb += add;
  dst->pix(dst_x, dst_y) = rgb.toRGB();
}

inline void recolor(RGBImage *img,
                     int x, int y,
                     float mul, int add) {
  WideRGB rgb(img->get(x, y));
  rgb.recolor(mul, add);
  img->pix(x, y) = rgb.toRGB();
}

inline void recolor(RGBImage *img,
                     int x, int y,
                     int add) {
  WideRGB rgb(img->get(x, y));
  rgb += add;
  img->pix(x, y) = rgb.toRGB();
}

void recolor_rectangle(RGBImage *img, BBox const &bbox,
                        float mul, int add) {
  for (int y = bbox.top(); y < bbox.bottom(); y++)
    for (int x = bbox.left(); x < bbox.right(); x++)
      recolor(img, x, y, mul, add);
}

void recolor_rect_edges(RGBImage *img, BBox const &bbox,
                         int bw,
                         int depth) {
  int x, y;
  int w = bbox.width() + 2 * bw, h = bbox.height() + 2 * bw;
  int x0 = bbox.left() - bw, y0 = bbox.top() - bw;
  int x1 = x0 + w - 1, y1 = y0 + h - 1;
  for (y = 1; y <= bw; y++)
    for (x = 0; x < w - y; x++) {
      recolor(img, x + x0, y + y0 - 1, 1., depth);
      recolor(img, x1 - x, y1 + 1 - y, 1., -depth);
    }
  for (x = 1; x <= bw; x++)
    for (y = bw; y < h - x; y++) {
      recolor(img, x + x0 - 1, y + y0, 1., depth);
      recolor(img, x1 - x + 1, y1 - y, 1., -depth);
    }
}

void recolor_circle(RGBImage *img,
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
        recolor(img, centre.x() + x, centre.y() + y,
                 mul, add);
      else
        recolor(img, centre.x() + x, centre.y() + y,
                 1., -depth * (y + x) / sqrt(r2));
    }
  }
}

void recolor_topline(RGBImage *dst,
                      RGBImage const *src, QPoint const &srctopleft,
                      int depth, bool /*left*/, bool right) {
  int bw = dst->height();
  int w = dst->width();
  for (int y = 1; y <= bw; y++)
    for (int x = w - 1 - right ? y : 0; x >= 0; x--)
      recolor(dst, src, x, y - 1,
               x + srctopleft.x(), y + srctopleft.y() - 1,
               depth);
}

void recolor_bottomline(RGBImage *dst,
                         RGBImage const *src,
                         QPoint const &srctopleft,
                         int depth, bool left, bool /*right*/) {
  int bw = dst->height();
  int w = dst->width();
  for (int y = 1; y <= bw; y++)
    for (int x = left ? y : 0; x < w; x++)
      recolor(dst, src, x, bw - y,
               x + srctopleft.x(), srctopleft.y() + bw - y,
               -depth);
}

void recolor_leftline(RGBImage *dst,
                       RGBImage const *src,
                       QPoint const &srctopleft,
                       int depth) {
  int bw = dst->width();
  int h = dst->height();
  for (int x = 0; x < bw; x++)
    for (int y = 0; y < h; y++)
      recolor(dst, src, x, y, x + srctopleft.x(), y + srctopleft.y(), depth);

}

void recolor_rightline(RGBImage *dst,
                       RGBImage const *src,
                       QPoint const &srctopleft,
                       int depth) {
  int bw = dst->width();
  int h = dst->height();
  for (int x = 0; x < bw; x++)
    for (int y = 0; y < h; y++)
      recolor(dst, src, x, y, x + srctopleft.x(), y + srctopleft.y(),
               -depth);
}
