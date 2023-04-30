// VisPit.H

#ifndef _VisPit_H
#define _VisPit_H

#include "FBPos.h"

class VisPit {
public:
  VisPit(int w, int h, class SBrickData const &sbd0,
         class BrickSprites const &bsp0,
         class BrickSprites const &bsp1);
  ~VisPit();
  class QPixmap const *cell(int x, int y) const {
    return map[x + y * wid];
  }
  bool changed(int x, int y) const;
  bool changedline(int y) const;
  void resetchanged();   // forget changes / mark updated
  void addbrick(FBPos const &pos, int id=-1, bool definitive=1);
  // for fight mode: addbrick(FBPos const &, int recolour); ?
  void rembrick(FBPos const &pos);
  void rembrick(int id);
  void pudding(int y, int dir=1);
  int width() const {
    return wid;
  }
  int height() const {
    return hei;
  }
  int cellsize() const;
  void clear();
private:
  void addrem(FBPos const &pos, bool addnotrem, bool usesec=false);
  void copyline(int dst, int src);
  void zeroline(int dst);
  class QPixmap const *&refcell(int x, int y) {
    return map[x + y * wid];
  }
  void setchanged(int x, int y);
  void setchanged(int y);
private:
  int wid, hei;
  class SBrickData const &sbd;
  class BrickSprites const &bsp;
  class BrickSprites const &bsp2;
  class QPixmap const **map;
  unsigned int *chmap;
  FBPos oldpos[2];
  bool op_inuse[2];
};
#endif
