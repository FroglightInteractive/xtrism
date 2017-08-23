// FBPos.H

#ifndef _FBPos_H
#define _FBPos_H

class FBPos {
public:
  FBPos(int bno1, int rot1, int x1, int y1):
    bno(bno1), rot(rot1), x(x1), y(y1) {
  }
  FBPos() {
    bno = rot = x = y = 0;
  }
  int bno, rot, x, y;
};

#endif