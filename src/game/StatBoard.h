// StatBoard.H

#ifndef _StatBoard_H
#define _StatBoard_H

#include <string>
#include <vector>
using namespace std;

#include "../gfx/GBox.h"
#include "../basics/Word.h"
#include "../pics/SharedBG.h"
#include "../poll/Sleeper.h"

class StatBoard: public GBox, public BGSharer, public Sleeper {
public:
  StatBoard(GBParent *p, int nlines, int labelw, int dataw,
            class TFont const &labelf, class TFont const &dataf,
                SharedBG &sbg, class PollServer &syncserv);
  void setlabel(int i, string const &txt, bool update=true);
  void setdata(int i, string const &txt, bool update=true);
  void setdata(int i, int val, bool update=true);
  void setdata(int i, double val, bool update=true);
  bool draw(Point const & origin, class ByteMap *bm);
  void redraw(const BBox &bbox);
  void poll();
  void cleardata();
private:
  void redrawlabel(int i, bool bg);
  void redrawdata(int i, bool bg);
  void ensure(unsigned int i);
  int numlines;
  int labelwidth, datawidth;
  class TFont const & labelfont;
  class TFont const & datafont;
  int dy0, dy;
  int dxl, dxd;
  vector<string> labels;
  vector<string> data;
  word updlabels, upddata;
};
#endif