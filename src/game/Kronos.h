// Kronos.H

#ifndef _Kronos_H
#define _Kronos_H

#include "GameTime.h"

class Kronos {
public:
  Kronos(int virtlines=0);
  void setlevel(int virtuallines);   // at start of game
  int getlevel() const {
    return virtuallines;
  }
  void updlevel(int lines);   // after pudding
  bool mustigodown() {
    return godown.ivalgone();
  }
  void notgonedown() {
    godown.againplease();
  }
  void resetgodown() {
    godown.reset();
  }
  void newbrick() {
    brick.reset();
    godown.reset();
  }
  int dt_godown() const {
    return godown.ival();
  }                                                    // } Referee will
  int dt_used() const {
    return brick.elapsed();
  }                                                    // } want to know
  bool safeival() const;   // for WishList to judge drop request
  void markpause(bool pause, bool godown_only=0);
  int nextdown() {
    return godown.next();
  }
private:
  GTimer godown, brick;
  int pausetime;
  int virtuallines;
  bool inispd;
};

#endif
