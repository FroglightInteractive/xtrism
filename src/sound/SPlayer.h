// SPlayer.h

#ifndef SPLAYER_H

#define SPLAYER_H

#include "Sample.h"
#include "Stereo.h"
#include <list>

struct NowPlaying {
  Sample *sample;
  int offset;
  float amp;
  float posn;
public:
  NowPlaying(Sample *sample, float amp, float posn):
    sample(sample), offset(0), amp(amp), posn(posn) {
  }
};
  
class SPlayer {
public:
  static SPlayer *instance();
  static void setInstance(SPlayer *);
  SPlayer();
  ~SPlayer();
  void play(Sample *s, float amp, float posn);
  void toggleSounds();
  void render(Stereo *dst, int nscans);
protected:
  bool enabled;
  std::list<NowPlaying> active;
protected:
  static SPlayer *_instance;
};

#endif
