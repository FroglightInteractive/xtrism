// LogPit.H

#include "LogPit.h"
#include "../bricks/data.h"
#include "../basics/dbx.h"

// Waker info: sends warn() whenever contents change.

unsigned int LogPit::fullline = ~0;
const unsigned int LP_LEFTMARG = 4;
const unsigned int LP_BOTTOMMARG = 4;

LogPit::LogPit(unsigned int w, unsigned int h, const SBrickData &sbdat):
  CellMatrix(h + LP_BOTTOMMARG), wid(w), hei(h), sbd(sbdat) {
  emptyline = 0xf | (~0 << (LP_LEFTMARG + wid));
  for (unsigned int i = 0; i < hei; i++)
    line(i) = emptyline;
  for (unsigned int i = hei; i < hei + LP_BOTTOMMARG; i++)
    line(i) = fullline;
}

LogPit::LogPit(const LogPit &lp):
  CellMatrix(lp),
  wid(lp.wid), emptyline(lp.emptyline), sbd(lp.sbd) {
}

LogPit::~LogPit() {
}

void LogPit::or_(int bno, int rot, int x, int y) {
  const BrickData &b(sbd.brick(bno, rot));
  for (unsigned int i = 0; i < b.cells(); i++)
    set(x + b.x(i), y + b.y(i));
}

void LogPit::bic(int bno, int rot, int x, int y) {
  const BrickData &b(sbd.brick(bno, rot));
  for (unsigned int i = 0; i < b.cells(); i++)
    set(x + b.x(i), y + b.y(i), 0);
}

bool LogPit::tst(int bno, int rot, int x, int y) const {
  dbx(-980804, "LP:tst bno,rot=%i,%i x,y=%i,%i", bno, rot, x, y);
  const BrickData &b(sbd.brick(bno, rot));
  for (unsigned int i = 0; i < b.cells(); i++)
    if (cell(x + b.x(i), y + b.y(i))) {
      dbx(-980804, "... -> true");
      return true;
    }
  dbx(-980804, "... -> false");
  return false;
}

void LogPit::collapseline(int y, int dir) {
  if (dir < 0) {
    for (unsigned int i = y; i < hei; i++)
      line(i) = line(i + 1);
    line(hei) = emptyline;
  } else {
    for (unsigned int i = y; i > 0; i--)
      line(i) = line(i - 1);
    line(0) = emptyline;
  }
}

int LogPit::findfullline() const {
  for (unsigned int y = hei - 1; y > 0; y--)
    if (linefull(y)) {
      dbx(-980804, "LP::ffl() -> %i", y);
      return y;
    }
  dbx(-980804, "LP::ffl() -> -1");
  return -1;
}

int LogPit::countfulllines() const {
  int n = 0;
  for (unsigned int y = hei - 1; y > 0; y--)
    n += linefull(y);
  dbx(-980804, "LP::cfl() -> %i", n);
  return n;
}
