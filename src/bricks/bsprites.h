// bsprites.H

#ifndef _bsprites_H
#define _bsprites_H

#include "Consts.h"
#include <QPixmap>
#include <QDir>
#include <QList>

class BrickSprites {
public:
  BrickSprites(class SBrickData const &sbd,
               QString cachedir,
               int size, int style=0);
  ~BrickSprites();
  QPixmap const &cell(int bno, int rot, int cel) const {
    return cells[cel + BD_MAXCELLS * (rot + BD_MAXROTS * bno)];
  }
  int size() const;
private:
  class BrickCell *docreate(int cel, class BrickCell const *base,
                            int size,
                            class RBrickData const &rbd, int rot);
private:
  int n;
  QList<QPixmap> cells;
};
#endif
