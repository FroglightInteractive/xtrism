// GameTime

#ifndef _GameTime_H
#define _GameTime_H

#include <math.h>

namespace GameTime {
  constexpr static int PRE_PUD=30;
  constexpr static int MID_PUD=40; // was 60 on arch, 30 in oldest X vsn
  int now();
  int dt_godown(int lines);
  int safeival(int virtlines);
};

class GTimer {
public:
  GTimer(int ival0=0);
  int elapsed() const {
    return GameTime::now() - last_;
  }
  int ival() const {
    return ival_;
  }
  int next() const {
    return next_;
  }
  void reset(int ival0=0);
  void adjust(int dt);
  bool ivalgone(bool faster=false);
  void againplease();
private:
  int last_;
  int next_;
  int ival_;
};

#endif
