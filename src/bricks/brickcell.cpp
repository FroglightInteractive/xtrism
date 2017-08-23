// brickcell.C

#include "brickcell.H"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "../basics/Throw.H"

BrickCell::BrickCell(unsigned int w, unsigned int h,
                     unsigned int depth): ByteMap(w, h ? h : w) {
  drawbasis(depth);
}

void BrickCell::drawborders(unsigned int bw, const BCSurround &sur,
                            int depth) {
  if (!sur.e) // east edge
    a_rect(wid - bw, bw, bw, hei - 2 * bw, -depth);

  switch (1 * sur.e + 16 * sur.n + 256 * sur.ne) { // ne corner
  case 0x000:
  case 0x100:
    a_tri_nw(wid - bw, 0, bw, depth);
    a_tri_se(wid - bw, 0, bw, -depth);
    break;

  case 0x001:
  case 0x101:
    a_rect(wid - bw, 0, bw, bw, depth);
    break;

  case 0x010:
  case 0x110:
    a_rect(wid - bw, 0, bw, bw, -depth);
    break;

  case 0x011:
    a_tri_nw(wid - bw, 0, bw, -depth);
    a_tri_se(wid - bw, 0, bw, +depth);
    break;

  case 0x111:
    break;
  }

  if (!sur.n) // north edge
    a_rect(bw, 0, wid - 2 * bw, bw, depth);

  switch (1 * sur.n + 16 * sur.w + 256 * sur.nw) { // nw corner
  case 0x000:
  case 0x100:
  case 0x011:
    a_line_nw_se(0, 0, bw, depth / 3); // fallthrough

  case 0x001:
  case 0x010:
  case 0x101:
  case 0x110:
    a_rect(0, 0, bw, bw, depth);
    break;

  case 0x111:
    break;
  }

  if (!sur.w) // west edge
    a_rect(0, bw, bw, hei - 2 * bw, depth);

  switch (1 * sur.w + 16 * sur.s + 256 * sur.sw) { // sw corner
  case 0x000:
  case 0x100:
    a_tri_nw(0, hei - bw, bw, depth);
    a_tri_se(0, hei - bw, bw, -depth);
    break;

  case 0x001:
  case 0x101:
    a_rect(0, hei - bw, bw, bw, -depth);
    break;

  case 0x010:
  case 0x110:
    a_rect(0, hei - bw, bw, bw, depth);
    break;

  case 0x011:
    a_tri_nw(0, hei - bw, bw, -depth);
    a_tri_se(0, hei - bw, bw, depth);
    break;

  case 0x111:
    break;
  }

  if (!sur.s) // south edge
    a_rect(bw, hei - bw, wid - 2 * bw, bw, -depth);

  switch (1 * sur.s + 16 * sur.e + 256 * sur.se) {
  case 0x000:
  case 0x011:
  case 0x100:
    a_line_nw_se(wid - bw, hei - bw, bw, -depth); // fallthrough

  case 0x001:
  case 0x010:
  case 0x101:
  case 0x110:
    a_rect(wid - bw, hei - bw, bw, bw, -depth);
    break;

  case 0x111:
    break;
  }
}

inline void BrickCell::a_rect(unsigned int x, unsigned int y,
                              unsigned int w, unsigned int h, int o) {
  for (unsigned int a = 0; a < h; a++)
    for (unsigned int b = 0; b < w; b++)
      alter(x + b, y + a, o);
}

inline void BrickCell::a_tri_ne(unsigned int x, unsigned int y,
                                unsigned int w, int o) {
  for (unsigned int a = 0; a < w; a++)
    for (unsigned int b = a + 1; b < w; b++)
      alter(x + b, y + a, o);
}

inline void BrickCell::a_tri_nw(unsigned int x, unsigned int y,
                                unsigned int w, int o) {
  for (unsigned int a = 0; a < w; a++)
    for (unsigned int b = 0; b < w - a - 1; b++)
      alter(x + b, y + a, o);
}

inline void BrickCell::a_tri_sw(unsigned int x, unsigned int y,
                                unsigned int w, int o) {
  for (unsigned int a = 0; a < w; a++)
    for (unsigned int b = 0; b < a; b++)
      alter(x + b, y + a, o);
}

inline void BrickCell::a_tri_se(unsigned int x, unsigned int y,
                                unsigned int w, int o) {
  for (unsigned int a = 0; a < w; a++)
    for (unsigned int b = w - a; b < w; b++)
      alter(x + b, y + a, o);
}

inline void BrickCell::a_line_nw_se(unsigned int x, unsigned int y,
                                    unsigned int w, int o) {
  for (unsigned int a = 0; a < w; a++)
    alter(x + a, y + a, o);
}

inline void BrickCell::a_line_ne_sw(unsigned int x, unsigned int y,
                                    unsigned int w, int o) {
  for (unsigned int a = 0; a < w; a++)
    alter(x + a, y + w - 1 - a, o);
}

inline unsigned int BrickCell::random(unsigned int w, unsigned int n,
                                      unsigned int nw) {
  int b
    = int(.37 * w + .37 * n + .25 * nw + .01 * 128 + 89.6 * rand() / RAND_MAX
          - 44.8);
  return max(min(b, 255), 0);
}

void BrickCell::drawstripes(unsigned int depth) {
  for (unsigned int y = 0; y < hei; y++) {
    for (unsigned int x = 0; x < wid; x++) {
      float phase = (y + x) * 10 * 3.141592 / sqrt(hei * hei + wid * wid);
      int a = int(cc(x, y) + depth * cos(phase));
      if (a < 0)
        a = 0;
      if (a > 255)
        a = 255;
      c(x, y) = a;
    }
  }
}

void BrickCell::drawbasis(unsigned int depth) {
  unsigned int x, y;
  for (y = 0; y < hei; y++)
    for (x = 0; x < wid; x++)
      c(x, y) = random(pixc(x - 1, y), pixc(x, y - 1), pixc(x - 1, y - 1));

  unsigned int a0 = (256 - depth) / 2;
  for (y = 0; y < hei; y++)
    for (x = 0; x < wid; x++)
      c(x, y) = a0 + depth * cc(x, y) / 256;
}