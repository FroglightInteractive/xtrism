// CellMatrix.C

#include "CellMatrix.h"
#include "../basics/dbx.h"

CellMatrix::CellMatrix(unsigned int height) {
  hei = height;
  data.resize(hei);
  clear();
}

void CellMatrix::clear() {
  data.fill(0);
}

CellMatrix::CellMatrix(CellMatrix const &oth) {
  hei = oth.hei;
  data = oth.data;
}

CellMatrix::~CellMatrix() {
}

void CellMatrix::set(unsigned int x, unsigned int y, bool set) {
  dbx(-980804, "CM:set x,y=%i,%i", x, y);
  if (x > 30)
    throw "CellMatrix::set() illegal x";
  if (set)
    line(y) |= (1UL << x);
  else
    line(y) &= ~(1UL << x);
}

void CellMatrix::or_(CellMatrix const &from,
                     int dx, int dy) {
  if (dx >= 0)
    for (int i = 0; i < int(from.hei); i++) {
      if (dy + i >= 0 && dy + i < int(hei))
        line(dy + i) |= from.line(i) << dx;
      else
        if (from.line(i) << dx)
          throw "CellMatrix::or() overflow";
    }
  else
    for (int i = 0; i < int(from.hei); i++) {
      if (dy + i >= 0 && dy + i < int(hei))
        line(dy + i) |= from.line(i) >> (-dx);
      else
        if (from.line(i) << dx)
          throw "CellMatrix::or() overflow";
    }
}

void CellMatrix::bic(CellMatrix const &from,
                     int dx, int dy) {
  if (dx >= 0)
    for (int i = 0; i < int(from.hei); i++) {
      if (dy + i >= 0 && dy + i < int(hei))
        line(dy + i) &= ~(from.line(i) << dx);
      else
        if (from.line(i) << dx)
          throw "CellMatrix::bic() overflow";
    }
  else
    for (int i = 0; i < int(from.hei); i++) {
      if (dy + i >= 0 && dy + i < int(hei))
        line(dy + i) &= ~(from.line(i) >> (-dx));
      else
        if (from.line(i) >> (-dx))
          throw "CellMatrix::bic() overflow";
    }
}

bool CellMatrix::tst(CellMatrix const &other, int dx, int dy) const {
  if (dx >= 0) {
    for (int i = 0; i < int(other.hei); i++) {
      if (dy + i >= 0 && dy + i < int(hei)) {
        if (line(dy + i) & (other.line(i) << dx))
          return true;
      }
    }
  } else {
    for (int i = 0; i < int(other.hei); i++) {
      if (dy + i >= 0 && dy + i < int(hei)) {
        if (line(dy + i) & (other.line(i) >> (-dx)))
          return true;
      }
    }
  }
  return false;
}
