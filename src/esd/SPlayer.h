// SPlayer.H

#ifndef _SPlayer_H
#define _SPlayer_H

#define SP_SOUNDDEV "/dev/dsp"
#define SP_DFLTSPD (44100 / 1)
#define SP_FRAGSPSEC 200
#define SP_MAXAHEAD .03

typedef int SPHandle;
const int MAXSAMPLES = 10;

#include "SampleExc.H"

class SPlayer {
public:
  SPlayer();
  ~SPlayer();
  SPHandle play(class Sample const *s, float freqratio,
                  float amplitude, float posn = 0 /* in [-1,1] */)
  throw (SampleExc);
  void stop(SPHandle const &n);
  void stop();
  void activate() throw (SampleExc);
  void deactivate();
  bool poll() throw (SampleExc);
private:
  void dostop();
private:
  bool dummy;
};

#endif