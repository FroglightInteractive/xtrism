// CellMatrix.H - base class for both LogPit and BrickData

#ifndef _CellMatrix_H
#define _CellMatrix_H

#include "../basics/Word.H"
#include "../basics/Throw.H"

class CellMatrix {
  public:
    CellMatrix(unsigned int height);
    CellMatrix(CellMatrix const &oth);
    virtual ~CellMatrix();
    void or_(CellMatrix const &from, int dx, int dy);
    void bic(CellMatrix const &from, int dx, int dy);
    bool tst(CellMatrix const &other, int dx, int dy) const;
    unsigned int height() const { return hei; }
    bool cell(unsigned x, unsigned y) const { return line(y) & (1UL<<x); }
  protected:
    void set(unsigned int x, unsigned int y, bool set=1);
    word &line(unsigned int y) { tthrow(y>=hei, "CellMatrix: bad access");
                                 return data[y]; }
    word const &line(unsigned int y) const { tthrow(y>=hei,
                                                    "CellMatrix: bad access");
                                             return data[y]; }
  private:
    word *data;
    unsigned int hei;
  };

#endif
