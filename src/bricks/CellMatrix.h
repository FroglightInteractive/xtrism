// CellMatrix.H - base class for both LogPit and BrickData

#ifndef _CellMatrix_H
#define _CellMatrix_H

#include <QVector>

class CellMatrix {
public:
  CellMatrix(unsigned int height);
  CellMatrix(CellMatrix const &oth);
  virtual ~CellMatrix();
  void or_(CellMatrix const &from, int dx, int dy);
  void bic(CellMatrix const &from, int dx, int dy);
  bool tst(CellMatrix const &other, int dx, int dy) const;
  void clear();
  unsigned int height() const {
    return hei;
  }
  bool cell(unsigned x, unsigned y) const {
    return line(y) & (1UL << x);
  }
protected:
  void set(unsigned int x, unsigned int y, bool set=1);
  unsigned int &line(unsigned int y) {
    Q_ASSERT_X(y<hei, "line", "CellMatrix: bad access");
    return data[y];
  }
  unsigned int const &line(unsigned int y) const {
    Q_ASSERT_X(y<hei, "cline", "CellMatrix: bad access");
    return data[y];
  }
private:
  QVector<unsigned int> data;
  unsigned int hei;
};

#endif
