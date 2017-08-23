// FallBrick.H

#ifndef _FallBrick_H
#define _FallBrick_H

#include "../bricks/data.h"
#include "FBPos.h"
#include "BrickEnv.h"

class FBResult {
public:
  FBResult(): moved(0), othermoved(0), hitother(0) {
  }
  byte moved : 1,
    othermoved : 1,
    hitother : 1;
};

class FBHResult: public FBResult {
public:
  FBHResult(): FBResult(), hitwall(0), shooshed(0) {
  }
  byte hitwall : 1,
    shooshed : 1;
};

class FBVResult: public FBResult {
public:
  FBVResult(): FBResult(), landed(0), dropped(0) {
  }
  byte landed : 1,
    dropped : 1;
};

class FallBrick {
public:
  FallBrick(const SBrickData &sbdat, int x_0, int y_0=0,
            int rot_0=0, int dy_0=0);
  BEClash newbrick(int bno);
  void setenv(class BrickEnv *e) {
    env = e;
  }
  bool clash(int bno1, int rot1, int x1, int y1);
  int dir() const {
    return dy;
  }

  FBHResult left() {
    return movex(-1, false);
  }
  FBHResult right() {
    return movex(+1, false);
  }
  FBHResult zleft() {
    return movex(-1, true);
  }
  FBHResult zright() {
    return movex(+1, true);
  }
  FBHResult rotate(int dir);
  FBVResult drop();
  FBVResult godown();
  FBVResult notifypudding(int y, int dir);

  FBPos const &position() const {
    return pos;
  }
protected:
  bool swap(FallBrick &other);   // these are _not_ commands: check flag
  bool godown_together(FallBrick &other);   // for our willingness!
  friend class BrickEnv_Team;
private:
  FBHResult movex(int dir, bool zap);
  bool tryshoosh(int dir);
private:
  FBPos pos;
  bool willing_to_godown_together;
  bool willing_to_swap;
  bool lastisright;
  const SBrickData &sbd;
  int x0, y0, rot0, dy;
  class BrickEnv *env;

  struct FBPoint {
    short x, y;
  };
  static FBPoint const moveforrotl[];
  static FBPoint const moveforrotr[];
};
#endif