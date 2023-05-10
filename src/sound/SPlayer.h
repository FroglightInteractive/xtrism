// SPlayer.h

#ifndef SPLAYER_H

#define SPLAYER_H

#include "Sample.h"
#include <QIODevice>
#include <list>

struct NowPlaying {
  Sample *sample;
  int offset;
  float amp;
  float posn;
public:
  NowPlaying(Sample *sample, float amp, float posn):
    sample(sample), offset(0), amp(amp), posn(posn) {
  }
};
  

class SPlayer: public QIODevice {
public:
  static SPlayer *instance();
  SPlayer();
  ~SPlayer();
  void play(Sample *s, float frqrat, float amp, float posn);
  void toggleSounds();
  qint64 bytesAvailable() const override;
  qint64 size() const override;
  bool atEnd() const override;
  bool isSequential() const override;
  qint64 readData(char *dst, qint64 maxsize) override;
  qint64 writeData(const char *data, qint64 len) override;
private:
  bool enabled;
  std::list<NowPlaying> active;
#if QT_VERSION >=0x060000
  class QAudioSink *sink;
#else
  class QAudioOutput *sink;
#endif
};

#endif
