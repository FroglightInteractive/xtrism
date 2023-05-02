// bricks/data.H
// shape data for bricks

/* A BrickData structure contains a bitmap of a single Trism Brick, in
   a specific rotation. It is initialized from a set of strings describing
   each of the lines in the brick.
 * An RBrickData structure contains 4 BrickData structures, one for each
   possible rotation. It is initialized from a FILE* from which lines
   describing the brick are read:
   - The first line contains: <brick number> <bricktype & colour description>
   - The next four (BD_LINES) lines contain bitmaps: -=0, other=1
   NB: 1. <brick number> is for debugging purposes only.
 * An SBrickData structure contains a number of RBrickData structures,
   one for each brick in a set of bricks.  It is initialized from a file
   which contains the number of bricks in the first line, followed
   by RBrick descriptions.
 * Note that none of these structures contain any information about the
   visible properties of a brick, except for the colour description in
   an RBrickData. This description however, is not used by (R|S)BrickData
   functions: it is for use by classes that create visible blocks using
   the logical information in the (R)BrickData.
 */

#ifndef _bricks_data_H
#define _bricks_data_H

#include "CellMatrix.h"
#include <QString>
#include <QTextStream>
#include "Consts.h"
#include <QStringList>
#include <QColor>

class BDLines: public QStringList {
public:
  BDLines(QStringList const &l): QStringList(l) {}
  inline bool safecell(int x, int y) const;
  unsigned int countcells() const;
};

class BrickData: public CellMatrix {
public:
  enum RotStyle {
    ROT_4x4=0,
    ROT_3x3=3,
    ROT_4x4_TWO=2,
  };
public:
  BrickData();
  BrickData(BDLines const &lines, int rot, RotStyle rotsty);
  ~BrickData();
  bool safecell(int x, int y) const {
    return (x < 0 || y < 0 || x >= int(BD_MAXSIZE) || y >= int(height()))
           ? false : cell(x, y);
  }
  bool collide(const BrickData &other, int dx, int dy) const
  // (dx,dy) is pos. of other, relative to us (+,+)=(right,down)
  {
    return tst(other, dx, dy);
  }
  void print() const;
  int x(unsigned int cel) const {
    return (cel < cells_) ? xx[cel] : -1;
  }
  int y(unsigned int cel) const {
    return (cel < cells_) ? yy[cel] : -1;
  }
  unsigned int cells() const {
    return cells_;
  }
private:
  unsigned int cells_;
  QVector<unsigned char> xx, yy;
};

class RBrickData: public QVector<BrickData> {
public:
  RBrickData();
  RBrickData(int rotstyle, QRgb rgb, QStringList lines);
  ~RBrickData();
  QRgb const &color() const {
    return color_;
  }
  BrickData::RotStyle rotstyle() const {
    return rotsty;
  }
  void print() const;
  unsigned int height() const {
    return (*this)[0].height();
  }
private:
  QRgb color_;
  BrickData::RotStyle rotsty;
};

class SBrickData: public QVector<RBrickData> {
public:
  SBrickData(std::initializer_list<RBrickData> const &src):
    QVector<RBrickData>(src) { }
  const BrickData &brick(int n, int rot) const {
    return (*this)[n][rot];
  }
  void print() const;
  unsigned int maxlines() const;
};

#endif
