// Recolour.H
// Make a part of a TImage lighter/darker/brighter/duller

#ifndef _Recolour_H
#define _Recolour_H

void recolour_rectangle(class TImage *img,
                          class BBox const &bbox,
                            float mul, int add);

void recolour_rect_edges(class TImage *img,
                           class BBox const &bbox,
                             int bw, // _outside_ the bbox!
                             int depth);

void recolour_circle(class TImage *img,
                       class Point const &centre,
                         int radius, float mul, int add,
                         int r1, int depth);

void recolour_topline(class TImage *dst,
                        class TImage const *src,
                          class Point const &srctopleft,
                            int depth, bool left, bool right);

void recolour_bottomline(class TImage *dst,
                           class TImage const *src,
                             class Point const &srctopleft,
                               int depth, bool left, bool right);

void recolour_leftline(class TImage *dst,
                         class TImage const *src,
                           class Point const &srctopleft,
                             int depth);

void recolour_rightline(class TImage *dst,
                          class TImage const *src,
                            class Point const &srctopleft,
                              int depth);

#endif