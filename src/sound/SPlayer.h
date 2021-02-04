// SPlayer.h

#ifndef SPLAYER_H

#define SPLAYER_H

#include "Sample.h"

class SPlayer {
public:
  SPlayer();
  ~SPlayer();
  void play(Sample *s, float frqrat, float amp, float posn);
  void toggleSounds();
private:
  bool enabled;
};

#endif
