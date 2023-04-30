// GameTime.C

#include "GameTime.h"
#include <QDateTime>

namespace GameTime {
  int now() {
    static QDateTime dt0 = QDateTime::currentDateTime();
    QDateTime dt = QDateTime::currentDateTime();
    return dt0.msecsTo(dt);
  }

  int dt_godown(int lines) {
    return int(770 * exp(-lines / 44.));
  }
  
  int safeival(int virtlines) {
    return int(1.5 * dt_godown(virtlines));
  }
};

GTimer::GTimer(int ival0):
  last_(GameTime::now()),
  next_(GameTime::now() + ival0),
  ival_(ival0) {
}

void GTimer::reset(int ival0) {
  if (ival0)
    ival_ = ival0;
  last_ = GameTime::now();
  next_ = last_ + ival_;
}

void GTimer::adjust(int dt) {
  last_ += dt;
  next_ += dt;
}

bool GTimer::ivalgone() {
  if (GameTime::now() >= next_) {
    next_ += ival_;
    return true;
  }
  return false;
}

void GTimer::againplease() {
  next_ = GameTime::now();
}
