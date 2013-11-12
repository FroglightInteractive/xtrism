// SPlayer.C

#include "SPlayer.H"
#include "Sample.H"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <esd.h>
#include "../basics/minmax.H"
#include "../basics/dbx.H"

SPlayer::SPlayer(): dummy(false) {
  int fd=esd_play_stream_fallback(ESD_BITS16|ESD_STEREO|ESD_STREAM|ESD_PLAY,
				  SP_DFLTSPD,
				  0,"trism");
  dummy = fd<0;
  if (fd>=0)
    close(fd);
  }

void SPlayer::activate() throw (SampleExc) {
  }

SPlayer::~SPlayer() {
  }

void SPlayer::deactivate() {
  }

SPHandle SPlayer::play(Sample const *s, float freqratio,
                       float amplitude, float posn) throw(SampleExc) {
  if (dummy) return -1;
  int pid = fork();
  if (pid<0) {
    perror("SPlayer::play: fork"); return 0;
  }
  if (pid>0) {
    // child
    int fd = esd_play_stream_fallback(ESD_BITS16|ESD_STEREO|ESD_STREAM|ESD_PLAY,
				      int(s->sfreq()*freqratio),
				      0,"trism_child");
    if (fd<0) {
      perror("Trism sound child: esd_play_stream");
      exit(1);
    }
    signed short buf[BUFSIZ/2];
    unsigned int i=0;
    float leftamp = amplitude*(1-posn)/2;
    float rightamp = amplitude*(1+posn)/2;
    while (i<s->length()) {
      int len = min(BUFSIZ/4, s->length() - i);
      signed short *bufp=buf;
      for (int j=0; j<len; j++) {
	float c=(*s)[i+j];
	float left=c*leftamp, right=c*rightamp;
	*bufp++=short(left);
        *bufp++=short(right);
      }
      i+=len;
      if (write(fd,buf,len*4)<0)
        { perror("SPlayer: write()"); throw SampleExc(); }
    }
    close(fd);
    exit(0);
  } // end of child
  return 1;
}

void SPlayer::stop(SPHandle const &sph) {
}
 
void SPlayer::stop() {
}
 
bool SPlayer::poll() throw (SampleExc) {
  return 0;
  }

