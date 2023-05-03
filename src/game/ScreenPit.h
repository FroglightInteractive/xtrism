// ScreenPit.H

#ifndef _ScreenPit_H
#define _ScreenPit_H

#include <QWidget>
#include "VisPit.h"
#include "Rainbow.h"
#include "XWorld.h"
#include "BrickSprites.h"

class ScreenPit: public QWidget {
public:
  ScreenPit(VisPit &vp, Rainbow const &sbg, QWidget *parent,
            BrickSprites const &bs0, BrickSprites const &bs1);
  /* In the GBArgs argument, the width and height members need not
     be specified; they are re-calculated from the VisPit's data. */
  ~ScreenPit();
  void paintEvent(QPaintEvent *) override;
  void poll();
private:
  void redrawcell(QPainter *p, int x, int y, QPixmap const *tsp) const;
  void generate();
private:
  Rainbow const &sharedbg;
  VisPit &vispit;
  QPixmap mybg;
  QPoint topleft;
  int wid, hei, bw;
private:
  class XWorld *xworld;
  XWorld::ID bgpixmap;
  QMap<QPixmap const *, XWorld::ID> brickpixmaps;
  BrickSprites const &bs0;
  BrickSprites const &bs1;
  void redrawxworld();
};

#endif
