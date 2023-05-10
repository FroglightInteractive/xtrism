// SPlayerPA.h

#ifndef SPLAYERPA_H

#define SPLAYERPA_H
#include "SPlayer.h"
#include "PAPlay.h"

class SPlayerPA: public SPlayer, private PAPlay {
public:
  SPlayerPA();
  ~SPlayerPA();
  Stereo const *provide(int nscans) override; 
private:
  PAPlay *backend;
  QVector<Stereo> buffer;
};

#endif
