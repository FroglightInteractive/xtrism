// SPlayer.cpp

#include "SPlayer.h"
#if QT_VERSION >= 0x060000
#include <QAudioDevices>
#include <QMediaDevices>
#include <QAudioSink>
#else
#include <QAudioDeviceInfo>
#include <QAudioOutput>
#endif
#include <QAudioFormat>
#include <QDebug>
#include <cstring>
#include <QFileInfo>
#include <QDebug>
#include <QProcess>

#define EXTERNALSOUND 1

SPlayer *SPlayer::instance() {
  static SPlayer spl;
  return &spl;
}

SPlayer::SPlayer() {
  enabled = false;
  sink = 0;
  toggleSounds();
}

void SPlayer::toggleSounds() {
  enabled = !enabled;
  if (enabled) {
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
#if QT_VERSION >= 0x060000
    format.setSampleFormat(QAudioFormat::Int16);
    QAudioDevice device(QMediaDevices::defaultAudioOutput());
    if (!device.isFormatSupported(format)) {
      qWarning() << "Cannot play sounds: Format not supported by backend.";
      return;
    }
    sink = new QAudioSink(device, format);
#else
    format.setSampleSize(16);
    format.setSampleType(QAudioFormat::SignedInt);
    QAudioDeviceInfo device(QAudioDeviceInfo::defaultOutputDevice());
    if (!device.isFormatSupported(format)) {
      qWarning() << "Cannot play sounds: Format not supported by backend.";
      return;
    }
    sink = new QAudioOutput(device, format);
#endif
    open(QIODevice::ReadOnly);
#if EXTERNALSOUND
#else
    sink->start(this);
#endif
  } else {
    delete sink;
    sink = 0;
    close();
  }
}

SPlayer::~SPlayer() {
  delete sink;
}


void SPlayer::play(Sample *s, float frqrat, float amp, float posn) {
  if (!enabled)
    return;
#if EXTERNALSOUND
  QString name(QFileInfo(s->name()).baseName());
  QString root = "/home/wagenaar/progs/xtrism/xtrism/data/sound";
  qDebug() << "play" << name;
  QProcess::startDetached("aplay",
                    QStringList{root + "/" + name + ".wav"});
#else
  active.push_front(NowPlaying(s, amp, posn));
#endif
}

qint64 SPlayer::bytesAvailable() const {
  return 2048;
}

qint64 SPlayer::size() const {
  return bytesAvailable();
}

bool SPlayer::atEnd() const {
  return false;
}

bool SPlayer::isSequential() const {
  return true;
}

qint64 SPlayer::readData(char *dst, qint64 maxsize) {
  if (maxsize<=0)
    return 0;
  int nscans = maxsize / 4;
  short *dst1 = (short*)(dst);
  std::memset(dst, 0, maxsize);
  std::list<NowPlaying>::iterator nxt;
  qDebug() << "splayer::readdata" << dst1 << maxsize << nscans;
  for (std::list<NowPlaying>::iterator it=active.begin();
       it!=active.end(); it=nxt) {
    nxt = it;
    ++nxt;
    int n = nscans;
    int offset = (*it).offset;
    int m = (*it).sample->size() - offset;
    bool islast = m <= n;
    if (islast)
      n = m;
    short *src = (*it).sample->data() + offset;
    short *dst2 = dst1;
    qDebug() << "active" << (*it).sample->data() << offset << (*it).sample->size() << n;
    float lamp = (*it).amp * (1 - (*it).posn)/2;
    float ramp = (*it).amp * (1 + (*it).posn)/2;
    (*it).offset += n;
    while (n--) {
      float s = *src++;
      *dst2++ += (short)(lamp * s);
      *dst2++ += (short)(ramp * s);
    }
    if (islast)
      active.erase(it);
  }
  return maxsize;
}

qint64 SPlayer::writeData(const char *data, qint64 len) {
  return len;
}
