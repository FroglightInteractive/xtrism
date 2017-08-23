// Sounds.C

#include "Sounds.H"
#include "Sample.H"
#include "SPlayer.H"
#include "../basics/Filename.H"
#include "../basics/dbx.H"

class Sounds *sounds = 0;

const float stdampl = .3;

Sounds::Sounds(Filename const &dir) throw (char const *) {
  sounds = this;
  speedup_ = turn_ = drop_ = applause_ = shoosh_ = explode_ = warn_ = 0;
  splayer = 0;
  try {
    splayer = new SPlayer();
    splayer->activate();
    splayer->deactivate();
  } catch (SampleExc) {
    fprintf(stderr, "Sounds: couldn't create SPlayer... sounds disabled");
    // sounds=0;
    return;   // NYI
  }
  try {
    speedup_ = new Sample((dir + "speedup").name());
    turn_ = new Sample((dir + "turn").name());
    drop_ = new Sample((dir + "drop").name());
    applause_ = new Sample((dir + "applause").name());
    shoosh_ = new Sample((dir + "shoosh").name());
    explode_ = new Sample((dir + "explode").name());
    warn_ = new Sample((dir + "warning").name());
  } catch (SampleExc) {
    throw "Sounds: Sample not found";
  }
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

bool Sounds::poll() {
  return splayer->poll();
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

void Sounds::activate() {
  splayer->activate();
}

void Sounds::deactivate() {
  splayer->deactivate();
}