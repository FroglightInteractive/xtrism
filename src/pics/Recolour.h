// Recolour.H
// Make a part of a RGBMap lighter/darker/brighter/duller

#ifndef _Recolour_H
#define _Recolour_H

void recolour_rectangle(class RGBMap *img,
                        class BBox const &bbox,
                        float mul, int add);

void recolour_rect_edges(class RGBMap *img,
                         class BBox const &bbox,
                         int bw, // _outside_ the bbox!
                         int depth);

void recolour_circle(class RGBMap *img,
                     class QPoint const &centre,
                     int radius, float mul, int add,
                     int r1, int depth);

void recolour_topline(class RGBMap *dst,
                      class RGBMap const *src,
                      class QPoint const &srctopleft,
                      int depth, bool left, bool right);

void recolour_bottomline(class RGBMap *dst,
                         class RGBMap const *src,
                         class QPoint const &srctopleft,
                         int depth, bool left, bool right);

void recolour_leftline(class RGBMap *dst,
                       class RGBMap const *src,
                       class QPoint const &srctopleft,
                       int depth);

void recolour_rightline(class RGBMap *dst,
                        class RGBMap const *src,
                        class QPoint const &srctopleft,
                        int depth);

#endif
