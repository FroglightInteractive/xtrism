// Kronos.C

#include "Kronos.h"
#include "GameTime.h"
#include "../basics/dbx.h"
#include "../sound/Sounds.h"

Kronos::Kronos(int virtlines):
  godown(GameTime::dt_godown(virtlines)),
  brick(0), pausetime(GameTime::now()),
  virtuallines(virtlines), inispd(true) {
}

void Kronos::setlevel(int virtlines) {
  virtuallines = virtlines;
  int dt = GameTime::dt_godown(virtuallines);
  godown.reset(dt);
}

void Kronos::updlevel(int lines) {
  if (lines > virtuallines) {
    setlevel(lines);
    if (inispd)
      Sounds::instance()->speedup();
    inispd = false;
  }
}

bool Kronos::safeival() const {
  return brick.elapsed() > GameTime::safeival(virtuallines);
}

void Kronos::markpause(bool pause, bool godown_only) {
  int t = GameTime::now();
  if (pause) {
    pausetime = t;
  } else {
    int dt = t - pausetime;
    if (godown.elapsed() < dt)   // godown reset during pause, ie setlevel
      godown.reset();   // if so, reset again
    else
      godown.adjust(dt);   // else just adjust for time spent paused
    if (!godown_only)
      brick.adjust(t - pausetime); // adjustment of dt_used is optional
    dbx(1, "Kronos::markpause(false): dt=%i\n", t - pausetime);
  }
}
