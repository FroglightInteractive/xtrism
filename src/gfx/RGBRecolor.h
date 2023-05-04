// RGBRecolor.H
// Make a part of a RGBImage lighter/darker/brighter/duller

#ifndef _RGBRecolor_H
#define _RGBRecolor_H

#include "RGBImage.h"
#include "BBox.h"

void recolor_rectangle(RGBImage *img,
                        BBox const &bbox,
                        float mul, int add);

void recolor_rect_edges(RGBImage *img,
                         BBox const &bbox,
                         int bw, // _outside_ the bbox!
                         int depth);

void recolor_circle(RGBImage *img,
                     QPoint const &centre,
                     int radius, float mul, int add,
                     int r1, int depth);

void recolor_topline(RGBImage *dst,
                      RGBImage const *src,
                      QPoint const &srctopleft,
                      int depth, bool left, bool right);

void recolor_bottomline(RGBImage *dst,
                         RGBImage const *src,
                         QPoint const &srctopleft,
                         int depth, bool left, bool right);

void recolor_leftline(RGBImage *dst,
                       RGBImage const *src,
                       QPoint const &srctopleft,
                       int depth);

void recolor_rightline(RGBImage *dst,
                        RGBImage const *src,
                        QPoint const &srctopleft,
                        int depth);

#endif
