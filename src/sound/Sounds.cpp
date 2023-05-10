// Sounds.C

#include "Sounds.h"
#include "Sample.h"
#include "../basics/dbx.h"
#include "SPlayer.h"
#include <iostream>

Sounds *Sounds::instance() {
  static Sounds snds;
  return &snds;
}

const float stdampl = .3;

Sounds::Sounds() {
  QString dir = ":/";
  speedup_ = turn_ = drop_ = applause_ = shoosh_ = explode_ = warn_ = 0;
 
  speedup_ = new Sample(dir + "speedup.raw");
  turn_ = new Sample(dir + "turn.raw");
  drop_ = new Sample(dir + "drop.raw");
  applause_ = new Sample(dir + "applause.raw");
  shoosh_ = new Sample(dir + "shoosh.raw");
  explode_ = new Sample(dir + "explode.raw");
  warn_ = new Sample(dir + "warning.raw");
}

Sounds::~Sounds() {
  delete speedup_;
  delete turn_;
  delete drop_;
  delete applause_;
  delete shoosh_;
  delete explode_;
  delete warn_;
}

void Sounds::speedup(float posn) const {
  SPlayer::instance()->play(speedup_, stdampl, posn);
}

void Sounds::turn(float posn) const {
  SPlayer::instance()->play(turn_, stdampl, posn);
}

void Sounds::drop(float posn) const {
   SPlayer::instance()->play(drop_, stdampl, posn);
}

void Sounds::applause(float posn) const {
   SPlayer::instance()->play(applause_, stdampl, posn);
}

void Sounds::shoosh(float posn) const {
  SPlayer::instance()->play(shoosh_, stdampl, posn);
}

void Sounds::explode(float posn, float ampl) const {
  SPlayer::instance()->play(explode_, ampl * stdampl, posn);
}

void Sounds::warn(float posn) const {
  SPlayer::instance()->play(warn_, stdampl, posn);
}


