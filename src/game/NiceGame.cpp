// NiceGame.C

// Sleeper info: NiceGame requests alarms in order to pudding.

const int CHLEV_MAXLINES = 9; // don't allow change level if > this # of lines
const int CHLEV_MAXLEV = 292; // superhuman speed: delay < 1 ms

#include <string>

#include "NiceGame.h"
#include "PlPlayer.h"
#include "LogPit.h"
#include "Score.h"
#include "Ranker.h"
#include "Keyboard.h"
#include "VisPit.h"
#include "ScreenPit.h"
#include "NextBox.h"
#include "StatBoard.h"
#include "GameTime.h"
#include "Session.h"
#include "../bricks/Probability.h"
#include "../env/TFont.h"
#include "../options/Player.h"
#include "../options/GlobalOpts.h"
#include "../bricks/data.h"
#include "../bricks/bsprites.h"
#include "../basics/dbx.h"

#include "../sound/Sounds.h"
#include "../env/TBusy.h"

const int SOLOWIDTH = 10;
const int SOLOHEIGHT = 22;
const int SOLOX0 = 8;

const int TEAMWIDTH = 14;
const int TEAMHEIGHT = 22;
const int TEAMX0 = 6;
const int TEAMX1 = 12;

NiceGame::NiceGame(Session &s, int pos0,
                   class PollServer &pollserv, class PollServer &syncserv,
                       Keyboard &kbd,
                       Player const *p1, Player const *p2,
                       GlobalOpts const &g,
                       SBrickData const &sbd0,
                       BrickSprites const &bs0, BrickSprites const &bs1,
                       int bset, ProbBSet const &pbs0,
                       TFont const &labelf, TFont const &dataf,
                       class Logger *logger):
  GBParent(&s.gbpar(), s.gbpar().width() / (pos0 ? 2 : 1),
           s.gbpar().height()),
  Game(s, kbd, g, sbd0, bs0, bs1, bset, &pbs0, logger), Sleeper(pollserv),
  pos(pos0), team(p2 != 0), lastpoll(0), pollserver(pollserv) {
  player[0] = p1;
  player[1] = p2;
  dbx(1, "NiceGame %p", this);
  int lw = labelf.stringwidth("Pts/Blk:");
  int dw = dataf.stringwidth("Heroic!");
  StatBoard *sb = new StatBoard(this, NLINES, lw, dw, labelf, dataf,
                                s, syncserv);
  VisPit *vp = new VisPit(team ? TEAMWIDTH : SOLOWIDTH,
                          team ? TEAMHEIGHT : SOLOHEIGHT,
                          sbd0, bs0, bs1);
  ScreenPit *sp = new ScreenPit(this, *vp, s, syncserv);
  Game::connect(sb, sp, vp);

  nextbox[0] = new NextBox(this, sbd0, bs0,
                           s, syncserv,
                           0, logger);
  nextbox[1] = team ? new NextBox(this, sbd0, bs1,
                                  s, syncserv,
                                  1, logger) : 0;

  statboard->setlabel(SCORE, "Score:", false);
  statboard->setlabel(LINES, "Lines:", false);
  statboard->setlabel(LEVEL, "Level:", false);
  statboard->setlabel(RANK, "Rank:", false);
  statboard->setlabel(PTSBLK, "Pts/Blk:", false);

  if (team)
    newchild(sp,                   // Placement of pit for 2 players:
             GBPos(nextbox[0], 1),  // left = 1 unit right of L. nextbox
             0,                    // top = undefined
             GBPos(nextbox[1], 1),  // right = 1 unit left of R. nextbox
             GBPos(this, height() / 10)); // bottom = 10% above bottom of screen
  else
    newchild(sp,                   // Placement of pit for 1 player
             GBPos(this, 1),        // left = 1 unit from left of screen
             0,                    // top = undefined
             GBPos(this, 1),        // right = 1 unit from right of screen
             GBPos(this, height() / 10)); // bottom = 10% above bottom of screen

  newchild(sb, GBPos(this, 1), GBPos(this, height() / 10), GBPos(sp, 1), 0);
  // positioning for 2p2p mode not yet OK!
  if (team) {
    newchild(nextbox[0], GBPos(this, 4), 0,
             GBPos(sp, 1), GBPos(sp));
    newchild(nextbox[1], GBPos(sp, 1), 0,
             GBPos(this, 4), GBPos(sp));
  } else {
    if (p1->nextpos() < 0)
      newchild(nextbox[0], GBPos(this, 5), 0, GBPos(sp, 1), GBPos(sp));
    else
      newchild(nextbox[0], GBPos(sp, 1), 0, GBPos(this, 5), GBPos(sp));
  }
  placechildren();
}

NiceGame::~NiceGame() {
  dbx(2, "~NiceGame %p", this);
  if (playing) {
    Game::quit(0);
  }
  for (int i = 0; i < nplrs; i++)
    plplayers[i]->disconnect(nextbox[i]);
  for (int i = 0; i < (team ? 2 : 1); i++)
    delete nextbox[i];
}

void NiceGame::start() {
  LogPit *lp = new LogPit(team ? TEAMWIDTH : SOLOWIDTH,
                          team ? TEAMHEIGHT : SOLOHEIGHT, sbd);
  nextbox[0]->clear();
  if (nextbox[1])
    nextbox[1]->clear();
  vispit->clear();
  statboard->setdata(SCORE, 0);
  statboard->setdata(LINES, lines = 0);
  statboard->setdata(LEVEL, player[0]->level(bset)); // should look at team lvl
  statboard->setdata(RANK, "-");
  statboard->setdata(PTSBLK, "-");
  Ranker *rk = new Ranker(); // incomplete...

  Game::connect(lp, rk);

  nplrs = team ? 2 : 1;
  for (int i = 0; i < nplrs; i++)
    plplayers[i] = new PlPlayer(this, team ? (-1 + 2 * i) : pos,
                                *player[i],
                                sbd, global, &keyboard, pollserver,
                                player[i]->level(bset), (*pbs)[bset],
                                team ? (i ? TEAMX1 : TEAMX0) : SOLOX0);
  for (int i = 0; i < nplrs; i++)
    plplayers[i]->connect(nextbox[i], *logpit, team ? plplayers[1 - i] : 0);

  Game::start();
}

extern void setlastscore(int sc, int li, double ppb, char const *name,
                         int bs);

void NiceGame::quit(bool natural) {
  setlastscore(score->pts(), lines, score->ppb(), player[0]->name().c_str(),
               bset);
  Game::quit(natural);
}

void NiceGame::addscore(double sc, PlPlayer *plp) {
  tbusy = true;
  // tthrow(plp!=plplayers[0], "NiceGame::addscore: Unexpected PlPlayer");
  (*score) += sc;
  dbx(1, "NiceGame::addscore %g", sc);
  statboard->setdata(SCORE, score->pts());
  statboard->setdata(PTSBLK, score->ppb());
}

void NiceGame::req_to_land(PlPlayer *plp) {
  if (pudding)
    landreq = plp;
  else
    plp->ok_to_land();
}

void NiceGame::bricklanded(FBPos const &pos, PlPlayer *plp) {
  showbrick(pos, plp, true);
  logpit->or_(pos.bno, pos.rot, pos.x, pos.y);
  puddreq = plp;
  pudding = 1;
  int i = GameTime::PRE_PUD;
  pudtime.reset(i);
  sendreq(pudtime.getnext());
  pudlns = 0;
}

void NiceGame::poll() {
  if (pudding && pudtime.ivalgone()) {
    int i = GameTime::MID_PUD;
    pudtime.adjust(i);
    sendreq(pudtime.getnext());
    int y = logpit->findfullline();
    if (y >= 0) {
      logpit->collapseline(y, pudding);
      vispit->pudding(y, pudding);
      sounds->explode();
      lines++;
      for (int j = 0; j < nplrs; j++) {
        plplayers[j]->updlevel(lines);
        plplayers[j]->notifypudding(y, 1);
      }
      statboard->setdata(LINES, lines);
      statboard->setdata(LEVEL, plplayers[0]->getlevel());
      pudlns++;
    } else {
      pudding = 0;
      if (puddreq) {
        puddreq->done_pudding(pudlns);
        puddreq = 0;
      }
      if (landreq) {
        landreq->ok_to_land();
        landreq = 0;
      }
    }
  }
}

void NiceGame::req_to_changelev(int change, PlPlayer *plp) {
  // check whether allowed
  int nlev = plp->getlevel() + change;
  if (nlev < lines)
    return; // don't allow if < lines

  if (lines > CHLEV_MAXLINES)
    return;

  if (nlev > CHLEV_MAXLEV)
    return;

  for (int i = 0; i < nplrs; i++)
    plplayers[i]->setlevel(nlev);
  statboard->setdata(LEVEL, nlev);
}

bool NiceGame::req_to_pause(PlPlayer *plp) {
  for (int i = 0; i < nplrs; i++)
    plplayers[i]->pause(plplayers[i] == plp);
  setpause(true);
  pauseowner = plp;
  return true;
}

bool NiceGame::req_to_unpause(PlPlayer *plp) {
  if (plp != pauseowner)
    return false;

  for (int i = 0; i < nplrs; i++)
    plplayers[i]->unpause(false);
  setpause(false);
  return true;
}

bool NiceGame::req_to_quit(bool dead, PlPlayer *plp) {
  selfquit(dead);
  return true;
}

void NiceGame::redraw(BBox const &bb) {
  dbx(1, "NiceGame::redraw bb=(%i,%i)-(%i,%i)", bb.left(), bb.top(),
      bb.right(), bb.bottom());
  GBParent::redraw(bb);
}