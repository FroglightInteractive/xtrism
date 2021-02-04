// SPlayer.cpp

#include "SPlayer.h"
#include <iostream>

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

