// GameTime.C

#include "GameTime.H"

int GameTime::dt_godown(int lines) {
  return int(770*exp(-lines/44.));
  }

int GameTime::safeival(int virtlines) {
  return int(1.5*dt_godown(virtlines));  
  }

GTimer::GTimer(int ival0): last(GameTime::now()),
  next(GameTime::now() + ival0), ival(ival0) {
  }

void GTimer::reset(int ival0) {
  if (ival0)
    ival=ival0;
  last=GameTime::now();
  next=last+ival;
  }

void GTimer::adjust(int dt) {
  last+=dt;
  next+=dt;
  }

bool GTimer::ivalgone() {
  if (GameTime::now()>=next)
    { next+=ival;
      return true;
    }
  return false;
  }

void GTimer::againplease() {
  next=GameTime::now();
  }
