// Game.H

// preliminary vsn, 1p1p only

#ifndef _Game_H
#define _Game_H

#include "GameTime.h"
#include "../env/TCallBack.h"

class Game: public TCallBack {
public:
  Game(class Session &s, class Keyboard &kbd, class GlobalOpts const &g,
             class SBrickData const &sbd0,
               class BrickSprites const &bs0, class BrickSprites const &bs1,
                   int bset0, class ProbBSet const *pbs0,
                     class Logger *logger);
  virtual ~Game();
private:
  // from GameForSession
  virtual void quit(bool natural);
  virtual void start();
  bool isplaying() {
    return playing;
  }
  friend class Session;
public:
  // from GameForPlPl // for PlPlayer ONLY
  void showbrick(class FBPos const & pos, class PlPlayer *plp,
                     bool definitive = false);
  void hidebrick(class FBPos const & pos, class PlPlayer *plp);
  virtual void req_to_land(class PlPlayer *plp) = 0;
  virtual void bricklanded(class FBPos const & pos,
                             class PlPlayer *plp) = 0;
  virtual void addscore(double sc, class PlPlayer *plp) = 0;
  virtual bool req_to_pause(class PlPlayer *plp) = 0;
  virtual bool req_to_unpause(class PlPlayer *plp) = 0;
  virtual void req_to_changelev(int change, class PlPlayer *plp) = 0;
  virtual bool req_to_quit(bool dead, class PlPlayer *plp) = 0;
private:
  // for descendents
  void selfquit(bool natural=true);
  void connect(class StatBoard *sb, class ScreenPit *sp,
                   class VisPit *vp);
  void connect(class LogPit *lp, class Ranker *rk);
  void setpause(bool onoff);
private:
  // really private functions
  void del_sb_sp_vp();
  void del_lp_rk_sc();
  void del_plpls();
  void callback(int natural);
private:
  // data for descendents
  class Session &session;
  class Keyboard &keyboard;
  class GlobalOpts const & global;
  class SBrickData const & sbd;
  class BrickSprites const & bs;
  class BrickSprites const & bs2;
  int bset;
  class ProbBSet const *pbs;
  bool playing;
  bool pause;

  class Score *score;   // my resp
  class Ranker *ranker;   // my resp
  // class Guinness *guinness;
  class StatBoard *statboard;   // my resp

  class LogPit *logpit;   // my resp
  class VisPit *vispit;   // my resp
  class ScreenPit *screenpit;   // myresp

  GTimer pudtime;
  int pudding;
  int pudlns;
  class PlPlayer *puddreq, *landreq;   // may be 0, not my resp
  class PlPlayer *plplayers[2];
  int nplrs;
  class Logger *logger;

  // friends:
  friend class NiceGame;
  friend class NiceReplay;
// friend class FightGame;
};

#endif