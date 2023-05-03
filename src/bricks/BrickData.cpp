// bricks/data.C

#include "BrickData.h"
#include <QFile>

const char BDL_EMPTY = '-';

// ================================ BDLines ==================================
inline bool BDLines::safecell(int x, int y) const {
  if (x<0 || y<0 || y>=size() || x>=(*this)[y].size())
    return false;
  return (*this)[y][x] != BDL_EMPTY;
}

unsigned int BDLines::countcells() const {
  int res = 0;
  for (unsigned int y = 0; y < size(); y++)
    for (unsigned int x = 0; x < BD_MAXSIZE; x++)
      res += safecell(x, y);
  return res;
}

// ======================= BrickData =========================================
inline int rotx(int x, int y, int rot, BrickData::RotStyle rotsty) {
  switch (rotsty) {
  case BrickData::ROT_4x4:
    rot &= 3;
    return (rot == 0) ? x : (rot == 3) ? y : (rot == 2) ? (3 - x) : (3 - y);

  case BrickData::ROT_4x4_TWO:
    return (rot & 1) ? (3 - y) : x;

  case BrickData::ROT_3x3:
    rot &= 3;
    return (rot == 0) ? x : (rot == 3) ? y : (rot == 2) ? (2 - x) : (2 - y);
  }
  throw "BrickData: rotx: illegal rotation style";
}

inline int roty(int x, int y, int rot, BrickData::RotStyle rotsty) {
  switch (rotsty) {
  case BrickData::ROT_4x4:
    rot &= 3;
    return (rot == 0) ? y : (rot == 3) ? (3 - x) : (rot == 2) ? (3 - y) : x;

  case BrickData::ROT_4x4_TWO:
    return (rot & 1) ? x : y;

  case BrickData::ROT_3x3:
    rot &= 3;
    return (rot == 0) ? y : (rot == 3) ? (2 - x) : (rot == 2) ? (2 - y) : x;
  }
  throw "BrickData: roty: illegal rotation style";
}

/* --------------------------- BrickData::BrickData ----------------------- */
/* Fills the data members.
 * lines is an array of strings describing each of the lines in the brick
 * rot is the angle to rotate by: 0/1/2/3. (1=90 deg clockwise)
 * rotsty is the rotation style
 */
BrickData::BrickData(): CellMatrix(BD_MAXSIZE) {
  cells_ = 0;
}

BrickData::BrickData(BDLines const &lines, int rot, RotStyle rotsty):
  CellMatrix(BD_MAXSIZE), cells_(lines.countcells()) {
  xx.resize(cells_);
  yy.resize(cells_);
  int n = (rotsty == ROT_4x4_TWO && (rot & 2)) ? cells_ - 1 : 0;
  int dn = n ? -1 : 1;
  for (unsigned int y = 0; y < height(); y++)
    for (unsigned int x = 0; x < BD_MAXSIZE; x++)
      if (lines.safecell(x, y)) {
        int rx = rotx(x, y, rot, rotsty);
        int ry = roty(x, y, rot, rotsty);
        set(rx, ry);
        xx[n] = rx;
        yy[n] = ry;
        n += dn;
      }
}

BrickData::~BrickData() {
}

/* -------------------------- BrickData::print ---------------------------- */
/* For debugging purposes: prints the contents of a BrickData. */
void BrickData::print() const {
  int j = 0;
  for (unsigned int y = 0; y < height(); y++) {
    for (unsigned int x = 0; x < BD_MAXSIZE; x++)
      if (line(y) & (1 << x)) {
        printf("#");
        j++;
      } else
        printf("-");
    printf("\n");
  }
  printf("num of cells = %i\n", j);
}

// ============================ RBrickData ===================================

RBrickData::RBrickData() {
  rotsty = BrickData::ROT_4x4;
}

RBrickData::RBrickData(int rotstyle, QRgb rgb, QStringList lines) {
  if (rotstyle==0)
    rotsty = BrickData::ROT_4x4;
  else if (rotstyle==2)
    rotsty = BrickData::ROT_4x4_TWO;
  else if (rotstyle==3)
    rotsty = BrickData::ROT_3x3;
  else
    throw "Illegal rotstyle";

  color_ = rgb;
  BDLines bdl(lines);
  for (unsigned int rot = 0; rot<4; rot++)
    *this << BrickData(bdl, rot, rotsty);
}

RBrickData::~RBrickData() {
}

void RBrickData::print() const {
  printf("Type = %i; Colour = %i %i %i\n", rotsty,
         qRed(color_), qGreen(color_), qBlue(color_));
  for (unsigned int r = 0; r < 4; r++) {
    (*this)[r].print();
    printf("\n");
  }
}

// ========================= SBrickData ======================================

void SBrickData::print() const {
  printf("Brickset containing %i bricks\n", int(size()));
  for (int i = 0; i < size(); i++) {
    printf("brick # %i:\n", i);
    (*this)[i].print();
  }
}

unsigned int SBrickData::maxlines() const {
  unsigned int mx = 0;
  for (int i = 0; i < size(); i++)
    mx = std::max(mx, (*this)[i].height());
  return mx;
}

#include "brickdefs.h"

SBrickData const &SBrickData::instance() {
  return gl_sbd;
}
