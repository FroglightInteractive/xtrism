// SPlayer.C

#include <esd.h>

#include "SPlayer.H"
#include "Sample.H"

#include <stdio.h>

#include "../basics/dbx.H"

SPlayer::SPlayer() {
  dummy = true;
  dbx(-990925,"Opening sound...");
  esd_handle = esd_open_sound(0);
  dbx(-990925,"Sound opened: %i",esd_handle);
  if (esd_handle < 0) {
    fprintf(stderr,"SPlayer: Cannot open ESD. Sound disabled.\n");
    return;
  }
  Sample::set_esd_handle(esd_handle);
  dummy = false;
}

SPlayer::~SPlayer() {
  if (!dummy) {
    esd_close(esd_handle);
  }
}

SPHandle SPlayer::play(Sample const *s, float freqratio,
		       float amplitude, float posn=0 /* in [-1,1] */)
  throw (SampleExc) {
  int id = s->id();
  esd_set_default_sample_pan(esd_handle, id, 
			     int(256*amplitude*(1.-posn)/2.),
			     int(256*amplitude*(1.+posn)/2.));
  esd_sample_play(esd_handle, id);
  return id;
}

void SPlayer::stop(SPHandle const &n) {
  esd_sample_stop(esd_handle, n);
}

void SPlayer::stop() { // Not implemented...
}

void SPlayer::activate() throw (SampleExc) {
}

void SPlayer::deactivate() {
}

bool SPlayer::poll() throw (SampleExc) {
  return false;
}
