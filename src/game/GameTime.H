// GameTime

#ifndef _GameTime_H
#define _GameTime_H

#include "../env/TTime.H"
#include <math.h>

class GameTime {
  public:
    enum { PRE_PUD=30 };
    enum { MID_PUD=40 }; // was 60 on arch, 30 in oldest X vsn
    static TTime now() { return TTime(TTime::CACHED); }
    static int dt_godown(int lines);
    static int safeival(int virtlines);
  };

class GTimer {
  public:
    GTimer(int ival0=0);
    int gettime() const { return GameTime::now() - last; }
    int getival() const { return ival; }
    TTime const &getnext() const { return next; }
    void reset(int ival0=0);
    void adjust(int dt);
    bool ivalgone();
    void againplease();
  private:
    TTime last;
    TTime next;
    int ival;
  };

#endif
