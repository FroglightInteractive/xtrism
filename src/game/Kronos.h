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
    return godown.getival();
  }                                                    // } Referee will
  int dt_used() const {
    return brick.gettime();
  }                                                    // } want to know
  bool safeival() const;   // for WishList to judge drop request
  void markpause(bool pause, bool godown_only=0);
  TTime const &getnextdown() {
    return godown.getnext();
  }
private:
  GTimer godown, brick;
  TTime pausetime;
  int virtuallines;
  bool inispd;
};

#endif