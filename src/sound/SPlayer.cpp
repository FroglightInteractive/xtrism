// SPlayer.cpp

#include "SPlayer.h"
#include <iostream>

SPlayer *SPlayer::instance() {
  static SPlayer spl;
  return &spl;
}

SPlayer::SPlayer() {
  enabled = true;
}

void SPlayer::toggleSounds() {
  enabled = !enabled;
}

SPlayer::~SPlayer() {
}


void SPlayer::play(Sample *s, float frqrat, float amp, float posn) {
  std::cerr << "play " << s << "\n";
  if (enabled)
    s->play();
}

