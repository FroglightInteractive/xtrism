// SPlayer.cpp

#include "SPlayer.h"
#include <QDebug>

SPlayer *SPlayer::_instance = 0;

SPlayer *SPlayer::instance() {
  if (_instance)
    return _instance;
  qDebug() << "No splayer - creating dummy";
  _instance = new SPlayer();
  return _instance;
}

void SPlayer::setInstance(SPlayer *s) {
  _instance = s;
}

SPlayer::SPlayer() {
  enabled = false;
  toggleSounds();
}

SPlayer::~SPlayer() {
}

void SPlayer::toggleSounds() {
  enabled = !enabled;
}

void SPlayer::play(Sample *s, float amp, float posn) {
  if (!enabled)
    return;
  active.push_front(NowPlaying(s, amp, posn));
}

void SPlayer::render(Stereo *dst, int nscans) {
  if (nscans<=0)
    return;
  std::memset(dst, 0, nscans*sizeof(Stereo));
  std::list<NowPlaying>::iterator nxt;
  qDebug() << "splayer::render" << dst << nscans;
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
    Stereo *dst2 = dst;
    qDebug() << "active" << (*it).sample->data() << offset
             << (*it).sample->size() << n;
    float lamp = (*it).amp * (1 - (*it).posn)/2;
    float ramp = (*it).amp * (1 + (*it).posn)/2;
    (*it).offset += n;
    while (n--) {
      float s = *src++;
      dst2->left += (short)(lamp * s);
      dst2->right += (short)(ramp * s);
      dst2++;
    }
    if (islast)
      active.erase(it);
  }
}
  
