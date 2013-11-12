// BrickEnv.C

#include "BrickEnv.H"
#include "FallBrick.H"
#include "LogPit.H"

BEClash BrickEnv::doiclash(int bno, int rot, int x, int y, bool /*wallonly*/) {
  return pit.tst(bno,rot,x,y)?BEC_SolidRock:BEC_OK;
  }

BEClash BrickEnv_Team::doiclash(int bno, int rot, int x, int y, bool wallonly) {
  BEClash bec(BrickEnv::doiclash(bno,rot,x,y));
  if (wallonly)
    return bec;
  if (bec==BEC_OK && other.clash(bno,rot,x,y))
    return bec=BEC_Brick;
  return bec;
  }

bool BrickEnv_Team::offerswap(class FallBrick &sender) {
  return other.swap(sender);
  }

bool BrickEnv_Team::offergodown(class FallBrick &sender) {
  return other.godown_together(sender);
  }
