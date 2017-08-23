// TTime.H

// Timeserver for Trism

#ifndef _TTime_H
#define _TTime_H

// TTime measures time in ms.

class TTime {
public:
  enum Setup {
    CURRENT, INFINITE_PAST, INFINITE_FUTURE,
    CACHED, MAGIC_DONTUSE
  };
  TTime(Setup const &x) {
    operator=(x);
  }
  TTime &operator=(Setup const &x);

  TTime &operator+=(int dt) {
    time += dt;
    return *this;
  }
  TTime &operator-=(int dt) {
    time -= dt;
    return *this;
  }
  TTime operator+(int dt) const {
    return TTime(time + dt);
  }
  TTime operator-(int dt) const {
    return TTime(time - dt);
  }

  int operator-(const TTime &t) const {
    return time - t.time;
  }

  bool operator<(const TTime &t) const {
    return time < t.time;
  }
  bool operator<=(const TTime &t) const {
    return time <= t.time;
  }
  bool operator>(const TTime &t) const {
    return time > t.time;
  }
  bool operator>=(const TTime &t) const {
    return time >= t.time;
  }

  int intrep() const {
    return time;
  }
private:
  int time;
  TTime(int t): time(t) {
  }
  static int cache;
};

#endif