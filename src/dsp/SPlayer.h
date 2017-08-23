// SPlayer.H

#ifndef _SPlayer_H
#define _SPlayer_H

#define SP_SOUNDDEV "/dev/dsp"
#define SP_DFLTSPD (44100 / 1)
#define SP_FRAGSPSEC 200
#define SP_MAXAHEAD .03

typedef int SPHandle;
const int MAXSAMPLES = 10;

#include "SampleExc.h"

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
  void polls16(int nsams) throw (SampleExc);
  void pollu8(int nsams) throw (SampleExc);
  void dostop(int k);
  void quiet() const;
  void donedeact();
private:
  int fd;
  bool dummy;
  void (SPlayer::*p)(int);
  bool stereo;
  unsigned int pfreq;
  unsigned int fragsams;
  unsigned int freefrags;
  int n;
  SPHandle lastsph;
  struct {
    class Sample const *sam;
    float ampl[2];
    float rate;
    float length;
    float offset;
    SPHandle sph;
  } sounds[MAXSAMPLES];
  /* ugly buffering stuff follows */
  bool deact;
  union {
    signed short *s16;
    unsigned char *u8;
    char *chr;
  } buf;
  int bufsize;
  /* end of ugly buffering stuff */
};

#endif