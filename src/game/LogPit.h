// LogPit.H

#ifndef _LogPit_H
#define _LogPit_H

#include "../bricks/CellMatrix.h"
#include "../basics/Word.h"
#include "../basics/Throw.h"
#include "../poll/Waker.h"

class LogPit: public CellMatrix, public Waker {
public:
  LogPit(unsigned int w, unsigned int h, class SBrickData const &sbdat);
  LogPit(const LogPit &lp);
  virtual ~LogPit();
  void or_(int bno, int rot, int x, int y);
  void bic(int bno, int rot, int x, int y);
  bool tst(int bno, int rot, int x, int y) const;   // LogPit & brick != 0
  bool linefull(unsigned int y) const {
    return line(y) == fullline;
  }
  void collapseline(int y, int dir);
  int findfullline() const;   // returns -1 if none found
  int countfulllines() const;
private:
  unsigned int wid, hei;
  word emptyline;
  class SBrickData const & sbd;
  static word fullline;
};

#endif