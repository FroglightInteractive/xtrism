// bsprites.H

#ifndef _bsprites_H
#define _bsprites_H

#include "Consts.H"

class BrickSprites {
public:
  BrickSprites(class SBrickData const &sbd, class Filename const &basename,
                   int size, class TEnv const &env, int style=0);
  ~BrickSprites();
  class TSprite const &cell(int bno, int rot, int cel) const {
    return *(cells[cel + BD_MAXCELL * (rot + BD_MAXROT * bno)]);
  }
  int size() const;
private:
  class BrickCell *docreate(int cel, class BrickCell const *mother,
                              int size,
                              class RBrickData const & rbd, int rot);
  // It's the callers resp to delete the create BrickCell!
private:
  int n;
  class TSprite **cells;
  bool *myresp;
};
#endif