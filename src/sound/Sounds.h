// Sounds.H - the collection of game sounds

#ifndef _Sounds_H
#define _Sounds_H

#include "Sample.h"

class Sounds {
public:
  Sounds(class Filename const &dir);
  ~Sounds();
  void speedup(float posn=0) const;
  void turn(float posn=0) const;
  void drop(float posn=0) const;
  void applause(float posn=0) const;
  void shoosh(float posn=0) const;
  void explode(float posn=0, float ampl=1) const;
  void warn(float posn=0) const;
  class SPlayer *player() {
    return splayer;
  }                                             // only used by Globals.C!
private:
  Sample *speedup_, *turn_, *drop_, *applause_;
  Sample *shoosh_, *explode_, *warn_;
  class SPlayer *splayer;
};

extern class Sounds *sounds;

#endif
