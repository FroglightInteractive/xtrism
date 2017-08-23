// FallBrick.C

#include "FallBrick.H"
#include "BrickEnv.H"
#include "../basics/Infty.H"
#include "../basics/dbx.H"

#include "../sound/Sounds.H"

FallBrick::FallBrick(const SBrickData &sbdat, int x_0, int y_0,
                     int rot_0, int dy_0):
  sbd(sbdat), x0(x_0), y0(y_0), rot0(rot_0), dy(dy_0) {
  willing_to_godown_together = willing_to_swap = 0;
}

BEClash FallBrick::newbrick(int bno) {
  pos.bno = bno;
  pos.rot = rot0;
  pos.x = x0;
  pos.y = y0;         // correction for certain bricks?
  return env->doiclash(pos.bno, pos.rot, pos.x, pos.y);
}

bool FallBrick::clash(int bno1, int rot1, int x1, int y1) {
  const BrickData &b0(sbd.brick(pos.bno, pos.rot));
  const BrickData &b1(sbd.brick(bno1, rot1));
  return b0.collide(b1, x1 - pos.x, y1 - pos.y);
}

bool FallBrick::tryshoosh(int dir) {
  int nx = pos.x + dir;
  BEClash bec;
  while (bec = env->doiclash(pos.bno, pos.rot, nx, pos.y), bec == BEC_Brick)
    nx += dir;
  if (bec == BEC_OK) {
    pos.x = nx;
    sounds->shoosh();
    return true;
  }   // true if moved OK, false if wall in the way
  return false;
}

FBHResult FallBrick::movex(int dir, bool zap) {
  FBHResult res;
  lastisright = dir > 0;
  // bool cont=true;
  do {
    switch (env->doiclash(pos.bno, pos.rot, pos.x + dir, pos.y)) {
    case BEC_OK:
      willing_to_swap = false;
      res.moved = 1;
      pos.x += dir;
      break;

    case BEC_Brick:
      if (tryshoosh(dir)) {
        willing_to_swap = false;
        res.moved = 1;
        res.shooshed = 1;
      } else if (env->offerswap(*this)) {
        willing_to_swap = false;
        res.moved = 1;
        res.shooshed = 1;
        res.othermoved = 1;
      } else {
        res.hitother = 1;
        willing_to_swap = true;
      }
      break;

    case BEC_SolidRock:
      willing_to_swap = false;
      res.hitwall = 1;
      break;
    }
  } while (zap && !res.hitwall && !res.hitother);
  return res;
}

FallBrick::FBPoint const FallBrick::moveforrotl[] = {
  { 0, 0 },
  { -1, 0 }, { 1, 0 }, { 0, 1 },
  { -1, 1 }, { 1, 1 },
  { -2, 0 }, { 2, 0 }, { 0, 2 },
};

FallBrick::FBPoint const FallBrick::moveforrotr[] = {
  { 0, 0 },
  { 1, 0 }, { -1, 0 }, { 0, 1 },
  { 1, 1 }, { -1, 1 },
  { 2, 0 }, { -2, 0 }, { 0, 2 },
};

FBHResult FallBrick::rotate(int dir) {
  FBHResult res;
  int nrot = (pos.rot + dir) & 3;
  BEClash bec;
  FBPoint const *mv = lastisright ? moveforrotr : moveforrotl;
  for (unsigned int i = 0;
       i < sizeof(moveforrotl) / sizeof(*moveforrotl);
       i++) {
    bec = env->doiclash(pos.bno, nrot,
                        pos.x + mv[i].x,
                        pos.y + mv[i].y);
    if (bec == BEC_OK) {
      pos.rot = nrot;
      pos.x += mv[i].x;
      pos.y += mv[i].y;
      res.moved = 1;
      sounds->turn();
      return res;
    }
  }
  if (bec == BEC_Brick)
    res.hitother = 1;
  else
    res.hitwall = 1;
  return res;
}

FBVResult FallBrick::drop() {
  FBVResult res;
  int y = pos.y;
  while (env->doiclash(pos.bno, pos.rot, pos.x, y + dy) != BEC_SolidRock)
    y += dy;
  // now y is lowest pos according to pit
  // must still check if clashing with other:
  if (env->doiclash(pos.bno, pos.rot, pos.x, y) == BEC_Brick) {
    res.hitother = 1;
  } else {
    res.moved = (y != pos.y);
    pos.y = y;
    res.landed = 1;
    res.dropped = 1;
  }
  // this method can be improved, but I prefer transparancy...
  return res;
}

FBVResult FallBrick::notifypudding(int y, int dir) {
  FBVResult res;
  dbx(1, "FallBrick::notifypudding(%i,%i)", y, dir);
  if ((y < pos.y + 1 && dir < 0) || (y > pos.y + 2 && dir > 0)) {
    /* if (above me & going up) or (below me & going down).
     * Note: pos.y+1/2 because I'm certainly occupying either of those
       lines, whatever my (bno,rot) - pos.y itself may be just above
       my head.
     */
    dbx(2, "FB:ntfpud: I move");
    pos.y += dir;
    res.moved = 1;
  }
  return res;
}

FBVResult FallBrick::godown() {
  FBVResult res;
  switch (env->doiclash(pos.bno, pos.rot, pos.x, pos.y + dy)) {
  case BEC_OK:
    pos.y += dy;
    res.moved = 1;
    willing_to_godown_together = false;
    break;

  case BEC_Brick:
    if (env->offergodown(*this)) {
      res.moved = 1;
      res.othermoved = 1;
      willing_to_godown_together = false;
    } else {
      res.hitother = 1;
      willing_to_godown_together = true;
    }
    break;

  case BEC_SolidRock:
    willing_to_godown_together = false;
    res.landed = 1;
    break;
  }
  return res;
}

bool FallBrick::swap(FallBrick &other) {
  if (!willing_to_swap)
    return false;

  if (lastisright == other.lastisright)
    return false;

  // We are willing to go, and agree on direction.
  // Clearly, we both have to move, otherwise this would not be happening.
  // Let's try to minimize movement.
  // So try in order: (1,1), (1,2), (2,1), (2,2), (1,3), (3,1).
  // Note that a total movement of more than four cannot be required.
  int mydx[] = { 1, 1, 2, 2, 1, 3 };
  int othdx[] = { 1, 2, 1, 2, 3, 1 };
  int dir = lastisright ? 1 : -1;
  for (int k = 0; k < 6; k++) {
    if (env->doiclash(pos.bno, pos.rot,
                      pos.x + dir * mydx[k], pos.y, true) == BEC_OK
        && other.env->doiclash(other.pos.bno, other.pos.rot,
                               other.pos.x - dir * othdx[k], pos.y,
                               true) == BEC_OK
        && !other.clash(pos.bno, pos.rot,
                        pos.x + dir * (mydx[k] + othdx[k]), pos.y)) {
      // Good!
      pos.x += dir * mydx[k];
      other.pos.x -= dir * othdx[k];
      willing_to_swap = false;
      return true;
    }
  }
  return false;
}

bool FallBrick::godown_together(FallBrick &other) { // dummy impl'n
  if (env->doiclash(pos.bno, pos.rot, pos.x, pos.y + dy, true) == BEC_OK
      && other.env->doiclash(other.pos.bno, other.pos.rot,
                             other.pos.x, other.pos.y + dy, true) == BEC_OK) {
    // We both could go!
    pos.y += dy;
    other.pos.y += dy;
    willing_to_godown_together = false;
    return true;
  }
  return false;
}