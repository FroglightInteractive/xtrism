// Sounds.C

#include "Sounds.h"
#include "Sample.h"
#include "../basics/dbx.h"
#include "SPlayer.h"
#include <iostream>

class Sounds *sounds = 0;

const float stdampl = .3;

Sounds::Sounds() {
  QString dir = "/home/wagenaar/progs/xtrism/xtrism/data/sounds/";
  sounds = this;
  speedup_ = turn_ = drop_ = applause_ = shoosh_ = explode_ = warn_ = 0;
  splayer = 0;
  splayer = new SPlayer();

  speedup_ = new Sample(dir + "speedup.wav");
  turn_ = new Sample(dir + "turn.wav");
  drop_ = new Sample(dir + "drop.wav");
  applause_ = new Sample(dir + "applause.wav");
  shoosh_ = new Sample(dir + "shoosh.wav");
  explode_ = new Sample(dir + "explode.wav");
  warn_ = new Sample(dir + "warning.wav");
}

Sounds::~Sounds() {
  if (splayer) delete splayer;
  if (speedup_) delete speedup_;
  if (turn_) delete turn_;
  if (drop_) delete drop_;
  if (applause_) delete applause_;
  if (shoosh_) delete shoosh_;
  if (explode_) delete explode_;
  if (warn_) delete warn_;
}

void Sounds::speedup(float posn) const {
  splayer->play(speedup_, 1, stdampl, posn);
}

void Sounds::turn(float posn) const {
  splayer->play(turn_, 1, stdampl, posn);
}

void Sounds::drop(float posn) const {
  splayer->play(drop_, 1, stdampl, posn);
}

void Sounds::applause(float posn) const {
  splayer->play(applause_, 1, stdampl, posn);
}

void Sounds::shoosh(float posn) const {
  splayer->play(shoosh_, 1, stdampl, posn);
}

void Sounds::explode(float posn, float ampl) const {
  splayer->play(explode_, 1, ampl * stdampl, posn);
}

void Sounds::warn(float posn) const {
  splayer->play(warn_, 1, stdampl, posn);
}


