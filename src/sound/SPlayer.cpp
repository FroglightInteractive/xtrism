// SPlayer.cpp

#include "SPlayer.h"
#include <iostream>

SPlayer::SPlayer() {
}

SPlayer::~SPlayer() {
}


void SPlayer::play(Sample *s, float frqrat, float amp, float posn) {
  std::cerr << "play " << s << "\n";
  
  s->play();
}

