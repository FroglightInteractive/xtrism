// ByteMap.C

#include <stdlib.h>
#include "ByteMap.h"
#include "../basics/dbx.h"
#include "../env/TCompress.h"
#include <string.h>

// #define SAFETY
#ifdef SAFETY
const int MARGIN = 1000;
#endif

ByteMap::ByteMap(unsigned int w, unsigned int h, byte ini):
  wid(w), hei(h) {
#ifdef SAFETY
  dat = new byte[wid * hei + 2 * MARGIN] + MARGIN;
  memset(dat - MARGIN, 0x35, wid * hei + 2 * MARGIN);
#else
  dat = new byte[wid * hei];
  memset(dat, ini, wid * hei);
#endif
}

ByteMap::ByteMap(unsigned int w, unsigned int h, TDecompress &src):
  wid(w), hei(h) {
#ifdef SAFETY
  dat = new byte[wid * hei + 2 * MARGIN] + MARGIN;
  memset(dat - MARGIN, 0x35, wid * hei + 2 * MARGIN);
#else
  dat = new byte[wid * hei];
#endif
  if (!src.read(dat, wid * hei)) {
    tthrow(src.eof(), "ByteMap: unexpected EOF");
    athrow("ByteMap: cannot read data");
  }
}

ByteMap::~ByteMap() {
#ifdef SAFETY
  dbx(3, "~ByteMap: safety check");
  for (int i = 0; i < MARGIN; i++)
    if (dat[wid * hei + i] != 0x35)
      athrow("~ByteMap: overflow past data");
  dat -= MARGIN;
  for (int i = 0; i < MARGIN; i++)
    if (dat[i] != 0x35)
      athrow("~ByteMap: overflow before data");
#endif
  delete[] dat;
}

ByteMap::ByteMap(ByteMap const &oth, unsigned int x, unsigned int y,
                 unsigned int w, unsigned int h): wid(w), hei(h) {
#ifdef SAFETY
  dat = new byte[wid * hei + 2 * MARGIN] + MARGIN;
  memset(dat - MARGIN, 0x35, wid * hei + 2 * MARGIN);
#else
  dat = new byte[wid * hei];
#endif
  for (unsigned int a = 0; a < hei; a++)
    memcpy(line(a), oth.cline(y + a) + x, wid);
}

ByteMap::ByteMap(ByteMap const &oth, int rot): wid(oth.wid), hei(oth.hei) {
  if (hei != wid)
    athrow("ByteMap: cannot rotate non-square map");
#ifdef SAFETY
  dat = new byte[wid * hei + 2 * MARGIN] + MARGIN;
  memset(dat - MARGIN, 0x35, wid * hei + 2 * MARGIN);
#else
  dat = new byte[wid * hei];
#endif
  switch (rot & 3) {
  case 0:
    memcpy(dat, oth.dat, wid * hei);
    break;

  case 1:
    for (unsigned int y = 0; y < hei; y++)
      for (unsigned int x = 0; x < wid; x++)
        c(x, y) = oth.cc(y, wid - 1 - x);
    break;

  case 2:
    for (unsigned int y = 0; y < hei; y++)
      for (unsigned int x = 0; x < wid; x++)
        c(x, y) = oth.cc(wid - 1 - x, hei - 1 - y);
    break;

  case 3:
    for (unsigned int y = 0; y < hei; y++)
      for (unsigned int x = 0; x < wid; x++)
        c(x, y) = oth.cc(wid - 1 - y, x);
    break;
  }
}

void ByteMap::write(TCompress &dst) const {
  tthrow(dst.write(dat, wid * hei), "ByteMap: cannot write data");
}