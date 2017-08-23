// NiceGame.H
// Both Solo and Team games.

#ifndef _NiceGame_H
#define _NiceGame_H

#include "Game.h"

#include "../gfx/GBParent.h"
#include "../poll/Sleeper.h"

class NiceGame: public GBParent, public Game, public Sleeper {
public:
  NiceGame(class Session &s, int pos0,
             class PollServer &pollserv, class PollServer &syncserv,
                 class Keyboard &kbd, // in future vsn perhaps 2 kbds?
                   class Player const *p1, class Player const *p2,
                       class GlobalOpts const &g,
                         class SBrickData const &sbd0,
                           class BrickSprites const &bs0,
                             class BrickSprites const &bs1,
                               int bset, class ProbBSet const &pbs0,
                                 class TFont const &labelf,
                                   class TFont const &dataf,
                                     class Logger *logger=0);
  virtual ~NiceGame();
  void start();
  void quit(bool natural);
  void poll();
  virtual void redraw(BBox const &bb);
private:
  // from GameForPlPl
  void addscore(double sc, class PlPlayer *plp);
  void req_to_land(class PlPlayer *plp);
  void bricklanded(class FBPos const & pos, class PlPlayer *plp);
  void req_to_changelev(int change, class PlPlayer *plp);
  bool req_to_pause(class PlPlayer *plp);
  bool req_to_unpause(class PlPlayer *plp);
  bool req_to_quit(bool dead, class PlPlayer *plp);
private:
  enum StatLines {
    SCORE=0, LINES, LEVEL, RANK, PTSBLK, NLINES,
  };
  class Player const *player[2];
  class NextBox *nextbox[2];
  int lines;
  int pos;
  bool team;
  bool lastpoll;
  class PlPlayer *pauseowner;
  class PollServer &pollserver;
};

#endif