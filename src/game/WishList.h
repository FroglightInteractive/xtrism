// WishList.H

#ifndef _WishList_H
#define _WishList_H

#include "FallBrick.h"
#include "KbdBuffer.h"
#include "Kronos.h"
#include "../basics/Byte.h"

class WLResult {
public:
  WLResult(): moved(0), othermoved(0), landed(0), dropped(0), gonedown(0) {
  }
  WLResult &operator|=(const FBHResult &fbh);
  WLResult &operator|=(const FBVResult &fbv);
  byte moved : 1, othermoved : 1, landed : 1, dropped : 1, gonedown : 1;
};

class WishList {
public:
  WishList(FallBrick &fb0, Kronos &kr0);
  bool logkey(BufferCode b);
  WLResult poll();
private:
  struct {
    byte left : 1, right : 1,
      zleft : 2, zright : 2,
      rotcw : 1, rotccw : 1,
      drop : 1;
  } movemark;
  FallBrick &fb;
  Kronos &kronos;
  struct {
    byte invol_land : 1, unexp_x : 1;
  } prot;
  void clearflags();
  void warn();
  void landed(bool dropped);   // called from poll() _only_
};
#endif