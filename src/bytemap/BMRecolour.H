// BMRecolour.H
// Make a part of a ByteMap lighter/darker/brighter/duller
// This code is almost entirely stolen from ../pics/Recolour.H vsn 1.1

#ifndef _BMRecolour_H
#define _BMRecolour_H

void recolour_rectangle(class ByteMap *img,
                          class BBox const &bbox,
                            float mul, int add);

void recolour_rect_edges(class ByteMap *img,
                           class BBox const &bbox,
                             int bw, // _outside_ the bbox!
                             int depth);

void recolour_circle(class ByteMap *img,
                       class Point const &centre,
                         int radius, float mul, int add,
                         int r1, int depth);

#endif