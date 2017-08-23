// Game.C

#include "Game.H"
#include "Score.H"
#include "Ranker.H"
#include "StatBoard.H"
#include "ScreenPit.H"
#include "VisPit.H"
#include "LogPit.H"
#include "Session.H"
#include "FBPos.H"
#include "GameTime.H"
#include "Logger.H"
#include "PlPlayer.H"

#include "../basics/dbx.H"

/* Notes:
 * StatBoard, ScreenPit, VisPit, and LogPit and Ranker are constructed
   by the descendants, ie SoloGame/TeamGame, but after they have been
   `connected' up, they are managed - and finally destructed by Game.
 * quit() and start() are called by Session, in response to user action,
   or indirectly through selfquit(). The descendants are responsible
   for proper implementation, and should not forget to call Game::quit()
   resp. Game::start()
 */

Game::Game(Session &s, Keyboard &kbd, GlobalOpts const &g,
           SBrickData const &sbd0,
           BrickSprites const &bs0, BrickSprites const &bs1,
           int bset0, ProbBSet const *pbs0, Logger *logger0):
  session(s), keyboard(kbd), global(g),
  sbd(sbd0), bs(bs0), bs2(bs1), bset(bset0), pbs(pbs0),
  pudding(0), puddreq(0), landreq(0),
  nplrs(0), logger(logger0) {
  dbx(1, "Game::Game");
  playing = pause = false;
  score = 0;
  ranker = 0;
  statboard = 0;
  logpit = 0;
  vispit = 0;
  screenpit = 0;

  session.reg_game(this);
}

Game::~Game() {
  dbx(1, "Game::~Game");
  session.dereg_game(this);
  del_sb_sp_vp();
  del_lp_rk_sc();
  del_plpls();
  session.env().dereg_callback(this);
  dbx(2, "Game::~Game done");
}

void Game::connect(class StatBoard *sb, class ScreenPit *sp,
                       class VisPit *vp) {
  del_sb_sp_vp();
  statboard = sb;
  screenpit = sp;
  vispit = vp;
}

void Game::del_sb_sp_vp() {
  if (statboard)
    delete statboard;
  statboard = 0;
  if (screenpit)
    delete screenpit;
  screenpit = 0;
  if (vispit)
    delete vispit;
  vispit = 0;
}

void Game::connect(class LogPit *lp, class Ranker *rk) {
  del_lp_rk_sc();
  logpit = lp;
  ranker = rk;
  if (rk)
    score = new Score;
}

void Game::selfquit(bool natural) {
  dbx(-980809, "Game::selfquit(%i)", natural);
  session.env().reg_callback(this, natural);
}

void Game::callback(int natural) {
  dbx(-980809, "Game::callback(%i)", natural);
  session.quit_game(this, natural);
}

void Game::del_lp_rk_sc() {
  if (logpit)
    delete logpit;
  logpit = 0;
  if (ranker)
    delete ranker;
  ranker = 0;
  if (score)
    delete score;
  score = 0;
}

void Game::del_plpls() {
  dbx(2, "Game::del_plpls, nplrs=%i", nplrs);
  for (int i = 0; i < nplrs; i++)
    delete plplayers[i];
  nplrs = 0;
}

void Game::start() {
  playing = true;
}

void Game::quit(bool natural) {
  dbx(1, "Game::quit(natural=%i)", natural);
  // if (score)
  // guinness->enter_hi();
  del_lp_rk_sc();
  del_plpls();
  if (logger)
    logger->quit();
  playing = false;
  dbx(2, "Game::quit done");
}

void Game::showbrick(FBPos const &pos, class PlPlayer *plp,
                       bool definitive) {
  vispit->addbrick(pos, plp != plplayers[0], definitive);
}

void Game::hidebrick(FBPos const & /*pos*/, class PlPlayer *plp) {
  vispit->rembrick(plp != plplayers[0]);
}

void Game::setpause(bool onoff) {
  if (onoff != pause) {
    pause = onoff;
    if (logger)
      logger->pause(pause);
  }
}