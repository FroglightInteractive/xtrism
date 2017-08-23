// PlPlayer.H - a playing player

#ifndef _PlPlayer_H
#define _PlPlayer_H

#include "KbdBuffer.h"
#include "Kronos.h"
#include "../poll/Sleeper.h"

class PlPlayer: public Sleeper {
public:
  PlPlayer(class Game *g, int pos,
             class Player const &p,
               class SBrickData const &sbd, class GlobalOpts const &glopts,
                   class Keyboard *kbd, class PollServer &pollserv,
                       int lvl, class Probability const &prob,
                         int x0, int y0=0, int rot0=0, int dy=1);
  virtual ~PlPlayer();
private:
  enum State {
    INIT, PLAYING, PAUSED, OTHPAUSED, DELLAND, LANDED, DELNEW, DEAD,
  };
  enum NBRes {
    OK, DELAY, DIE,
  };
  NBRes newbrick();
  void poll_playing();
  bool globalkey(BufferCode &bc);
  void poll_pause();
  void poll_delnew();
  void land(bool drop);
  void setstate(State s);
public:
  // from PlPlForGame // for Game and descendents ONLY
  void ok_to_land();
  void done_pudding(int lns=-1);
  void notifypudding(int y, int dir);
  int getlevel() {
    return kronos->getlevel();
  }
  void setlevel(int virtlines) {
    kronos->setlevel(virtlines);
  }
  void updlevel(int lines) {
    kronos->updlevel(lines);
  }
  void pause(bool own);
  void unpause(bool payforit);
  void connect(class NextBox *nb, class LogPit &lp, PlPlayer *oth = 0);
  void disconnect(class NextBox *nb);
  void disconnect(PlPlayer *oth);
  void poll();
private:
  class Game *game;
  class PlPlayer *other;   // may be 0

  class Player const & player;
  class Kronos *kronos;   // my resp
  class FallBrick *fallbrick;   // my resp
  class BrickEnv *brickenv;   // my resp
  class Referee *referee;   // my resp
  class KbdBuffer *kbdbuffer;   // my resp
  class ControlBuf *ctrlbuf;   // my resp
  class WishList *wishlist;   // my resp
  class Fortuna *fortuna;   // my resp
  class NextBox *nextbox;   // may be 0

  int nextbno;
  State state;
  int yland;
  bool queued_okland, queued_donepud;
  class GTimer *levup_autorep, *levdn_autorep;
};

#endif