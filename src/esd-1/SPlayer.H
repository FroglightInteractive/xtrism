// SPlayer.H

#ifndef _SPlayer_H
#define _SPlayer_H

#include "SampleExc.H"

typedef int SPHandle;

class SPlayer {
public:
  SPlayer();
  ~SPlayer();
  SPHandle play(class Sample const *s, float freqratio,
		float amplitude, float posn=0 /* in [-1,1] */)
    throw (SampleExc);
  void stop(SPHandle const &n);
  void stop();
  void activate() throw (SampleExc);
  void deactivate();
  bool poll() throw (SampleExc);
private:
  bool dummy;
  int esd_handle;
};

#endif
