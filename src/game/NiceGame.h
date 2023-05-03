// NiceGame.H
// Both Solo and Team games.

#ifndef _NiceGame_H
#define _NiceGame_H

#include <QObject>
#include "GameTime.h"
#include "Sides.h"

class NiceGame: public QObject {
  Q_OBJECT;
public:
  NiceGame(class NiceSession *s, Sides::Side pos0,
           class Player const *p1, class Player const *p2,
           class MetaKeys const *mk1, class MetaKeys const *mk2,
           class SBrickData const &sbd0,
           class BrickSprites const *bs0, class BrickSprites const *bs1,
           int bset);
  virtual ~NiceGame();
  void start();
  void terminate(bool natural);
  bool isplaying() {
    return playing;
  }
public:
  void timerEvent(QTimerEvent *) override;
signals:
  void quit();
public:
  void addscore(double sc, class PlPlayer *plp);
  void req_to_land(class PlPlayer *plp);
  void bricklanded(class FBPos const & pos, class PlPlayer *plp);
  void req_to_changelev(int change, class PlPlayer *plp);
  bool req_to_pause(class PlPlayer *plp);
  bool req_to_unpause(class PlPlayer *plp);
public:
  // from Game
  void showbrick(class FBPos const & pos, class PlPlayer *plp,
                     bool definitive = false);
  void hidebrick(class FBPos const & pos, class PlPlayer *plp);
  void selfquit(bool natural=true);
  void setpause(bool onoff);
public:
  void key(int code, bool in_not_out);
private:
  enum StatLines {
    SCORE=0, LINES, LEVEL, /*RANK,*/ PTSBLK, NLINES,
  };
  class Player const *player[2];
  class NextBox *nextbox[2];
  int lines;
  Sides::Side pos;
  bool team;
  bool lastpoll;
  class PlPlayer *pauseowner;
private:
  // from Game
  class NiceSession *session;
  class SBrickData const &sbd;
  class BrickSprites const *bs;
  class BrickSprites const *bs2;
  int bset;
  bool playing;
  bool pause;

  class Score *score;   // my resp
  class Ranker *ranker;
  class StatBoard *statboard;   // my resp
  class LogPit *logpit;   // my resp
  class VisPit *vispit;   // my resp
  class ScreenPit *screenpit;   // myresp

  GTimer pudtime;
  int pudding;
  int pudlns;
  class PlPlayer *puddreq, *landreq;   // may be 0, not my resp
  class PlPlayer *plplayers[2];
  class MetaKeys const *mkeys[2];
  int nplrs;
  int timerid;
};

#endif
