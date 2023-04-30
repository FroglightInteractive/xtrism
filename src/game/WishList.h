// WishList.H

#ifndef _WishList_H
#define _WishList_H

#include "FallBrick.h"
#include "KbdBuffer.h"
#include "Kronos.h"

class WLResult {
public:
  WLResult(): moved(0), othermoved(0), landed(0), dropped(0), gonedown(0) {
  }
  WLResult &operator|=(const FBHResult &fbh);
  WLResult &operator|=(const FBVResult &fbv);
  bool moved, othermoved, landed, dropped, gonedown;
};

class WishList {
public:
  WishList(FallBrick &fb0, Kronos &kr0);
  bool logkey(BufferCode b);
  WLResult poll();
private:
  struct {
    bool left, right;
    unsigned char zleft, zright;
    bool rotcw, rotccw, drop;
  } movemark;
  FallBrick &fb;
  Kronos &kronos;
  struct {
    bool invol_land, unexp_x;
  } prot;
  void clearflags();
  void warn();
  void landed(bool dropped);   // called from poll() _only_
};
#endif
