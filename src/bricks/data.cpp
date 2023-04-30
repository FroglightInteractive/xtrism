// bricks/data.C

#include "data.h"
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
  athrow("BrickData: rotx: illegal rotation style");
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
  athrow("BrickData: roty: illegal rotation style");
}

/* --------------------------- BrickData::BrickData ----------------------- */
/* Fills the data members.
 * lines is an array of strings describing each of the lines in the brick
 * rot is the angle to rotate by: 0/1/2/3. (1=90 deg clockwise)
 * rotsty is the rotation style
 */
BrickData::BrickData(BDLines const &lines, int rot, RotStyle rotsty):
  CellMatrix(BD_MAXSIZE), cells_(lines.countcells()), xx(0), yy(0) {
  xx = new unsigned char[cells_];
  yy = new unsigned char[cells_];
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
  delete[] yy;
  delete[] xx;
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

RBrickData::RBrickData(QTextStream *src, int t): bd(0) {
  QString buf = src->readLine().split("-")[0].trimmed();
  while (buf.isEmpty())
    buf = src->readLine().split("-")[0].trimmed();
  QStringList bits = buf.split(" ");
  if (bits.size()<2)
    throw("RBrickData: Bad brick data");
  if (bits[0].toInt() != t)
    throw("RBrickData: Bad brick number");

  rots = 4;
  QStringList cbits = bits[2].split(",");
  colour_ = RGB(255.999*cbits[0].toFloat(),
                255.999*cbits[1].toFloat(),
                255.999*cbits[2].toFloat());
  switch (bits[1].toInt()) {
    case 0:
      rotsty = BrickData::ROT_4x4;
      break;
    case 2:
      rotsty = BrickData::ROT_4x4_TWO;
      break;
    case 3:
      rotsty = BrickData::ROT_3x3;
      break;
    default:
      throw "RBrickData: Illegal rotation style";
    }

  BDLines lines;
  for (unsigned int y = 0; y < BD_MAXSIZE; y++)
    lines << src->readLine();

  for (unsigned int rot = 0; rot < rots; rot++)
    bd[rot] = new BrickData(lines, rot, rotsty);
}

RBrickData::~RBrickData() {
  for (unsigned int i = 0; i < rots; i++)
    if (bd[i])
      delete bd[i];
}

void RBrickData::print() const {
  printf("Type = %i; Colour = %s\n", rotsty, colour_.c_str());
  for (unsigned int r = 0; r < rots; r++) {
    bd[r]->print();
    printf("\n");
  }
}

// ========================= SBrickData ======================================

SBrickData::SBrickData(QString const &filename): rbd(0) {
  QFile f(filename);
  if (!f.open())
    throw QString("SBrickData: Couldn't open “" + filename + "”");
  QTextStream ts(&f);
  QString txt = ts.readLine().split("-")[0].trimmed();
  bool ok;
  n = txt.toInt(&ok);
  if (n<=0 || !bok)
    throw "SBrickData: Couldn't determine number of bricks";
  rbd = new RBrickData *[n];
  for (int i = 0; i < n; i++)
    rbd[i] = new RBrickData(&ts, i);
}

SBrickData::~SBrickData() {
  for (int i = 0; i < n; i++)
    delete rbd[i];
  delete[] rbd;
}

void SBrickData::print() const {
  printf("Brickset containing %i bricks\n", n);
  for (int i = 0; i < n; i++) {
    printf("brick # %i:\n", i);
    rbd[i]->print();
  }
}

unsigned int SBrickData::maxlines() const {
  unsigned int mx = 0;
  for (int i = 0; i < n; i++)
    mx = max(mx, rbd[i]->height());
  return mx;
}
