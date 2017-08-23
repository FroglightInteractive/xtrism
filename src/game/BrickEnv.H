// BrickEnv.H

#ifndef _BrickEnv_H
#define _BrickEnv_H

enum BEClash {
  BEC_OK=0,
  BEC_SolidRock=1,
  BEC_Brick=2
};

class BrickEnv {
public:
  BrickEnv(class LogPit &lp): pit(lp) {
  }
  virtual BEClash doiclash(int bno, int rot, int x, int y,
                           bool wallonly=false);
protected:
  virtual bool offerswap(class FallBrick & /*sender*/) {
    return false;
  }
  virtual bool offergodown(class FallBrick & /*sender*/) {
    return false;
  }
  class LogPit &pit;
  friend class FallBrick;
};

class BrickEnv_Solo: public BrickEnv {
public:
  BrickEnv_Solo(class LogPit &lp):
    BrickEnv(lp) {
  }
protected:
  friend class FallBrick;
};

class BrickEnv_Team: public BrickEnv {
public:
  BrickEnv_Team(class LogPit &lp, class FallBrick &otherfb):
    BrickEnv(lp), other(otherfb) {
  }
  BEClash doiclash(int bno, int rot, int x, int y, bool wallonly=false);
  bool offerswap(class FallBrick &sender);
  bool offergodown(class FallBrick &sender);
protected:
  class FallBrick &other;
  friend class FallBrick;
};

#endif