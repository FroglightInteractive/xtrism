// Replay.H

#ifndef _Replay_H
#define _Replay_H

#include "../env/TTime.h"

class Replay {
public:
  Replay(class Logger *logger, class VisPit *vp,
             class NextBox *nb0, class NextBox *nb1);
  int step(TTime const &t);   // +ve if something done, -ve if quit
private:
  unsigned int nextoffset;
  class LNode *data;      // not privately owned!
  TTime nexttime;
  bool quit;
  class VisPit *vp;       // id.
  class NextBox *nb[2];   // id
};
#endif