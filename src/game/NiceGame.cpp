// NiceGame.C

// Sleeper info: NiceGame requests alarms in order to pudding.

const int CHLEV_MAXLINES = 9; // don't allow change level if > this # of lines
const int CHLEV_MAXLEV = 292; // superhuman speed: delay < 1 ms

extern void setlastscore(int sc, int li, double ppb, char const *name, int bs);

#include <string>

#include "NiceGame.h"
#include "PlPlayer.h"
#include "LogPit.h"
#include "Score.h"
#include "Ranker.h"
#include "VisPit.h"
#include "ScreenPit.h"
#include "NextBox.h"
#include "StatBoard.h"
#include "GameTime.h"
#include "NiceSession.h"
#include "../bricks/Probability.h"
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

NiceGame::NiceGame(NiceSession *s, int pos0,
                   Player const *p1, Player const *p2,
                   GlobalOpts const &g,
                   SBrickData const &sbd0,
                   BrickSprites const &bs0, BrickSprites const &bs1,
                   int bset0):
  session(s), global(g),
  sbd(sbd0), bs(bs0), bs2(bs1), bset(bset0), 
  pudding(0), puddreq(0), landreq(0),
  nplrs(0),
  pos(pos0), team(p2!=0) {
  player[0] = p1;
  player[1] = p2;
  playing = pause = false;
  dbx(1, "NiceGame %p", this);
  int lw = QFontMetrics(s->font()).horizontalAdvance("Pts/Blk:");
  int dw = QFontMetrics(s->font()).horizontalAdvance("Heroic!");
  statboard = new StatBoard(NLINES, lw, dw, 
                            s->background(), s);
  logpit = new LogPit(team ? TEAMWIDTH : SOLOWIDTH,
                      team ? TEAMHEIGHT : SOLOHEIGHT, sbd);
  vispit = new VisPit(team ? TEAMWIDTH : SOLOWIDTH,
                      team ? TEAMHEIGHT : SOLOHEIGHT,
                      sbd0, bs0, bs1);
  screenpit = new ScreenPit(*vispit, s->background(), s);
  nextbox[0] = new NextBox(sbd0, bs0, s->background(), s);
  nextbox[1] = team ? new NextBox(sbd0, bs1, s->background(), s) : 0;

  statboard->setlabel(SCORE, "Score:", false);
  statboard->setlabel(LINES, "Lines:", false);
  statboard->setlabel(LEVEL, "Level:", false);
  statboard->setlabel(RANK, "Rank:", false);
  statboard->setlabel(PTSBLK, "Pts/Blk:", false);

  ranker = new Ranker(); // incomplete...
  score = new Score();

  
  QSize size = QSize(s->width() / (pos?2:1), s->height());
  QPoint topleft(pos>0 ? size.width() : 0, 0);
  QRect bbox(topleft, size);
  screenpit->move(bbox.center()
           - QPoint(screenpit->width()/2, screenpit->height()/2)
           - QPoint(0, size.height()/20));
  statboard->move(screenpit->x()/2 - statboard->width()/2,
                  screenpit->y()/2 - statboard->width()/2);

  if (team) {
    nextbox[0]->move(screenpit->x() - nextbox[0]->width() - size.width()/10,
                     screenpit->y() + screenpit->height()
                     - nextbox[0]->height());
    nextbox[1]->move(screenpit->x() + screenpit->width() + size.width()/10,
                     screenpit->y() + screenpit->height()
                     - nextbox[1]->height());

  } else {
    nextbox[0]->move(screenpit->x() - nextbox[0]->width() - size.width()/10,
                     screenpit->y() + screenpit->height()
                     - nextbox[0]->height());
    nextbox[0]->move(screenpit->x() + screenpit->width() + size.width()/10,
                     screenpit->y() + screenpit->height()
                     - nextbox[0]->height());
  }
  timerid = -1;
}

NiceGame::~NiceGame() {
  dbx(2, "~NiceGame %p", this);
  if (playing) 
    terminate(false);
  delete nextbox[0];
  delete nextbox[1];
  delete screenpit;
  delete vispit;
  delete logpit;
  delete statboard;
  delete score;
  delete ranker;
  for (int i=0; i<nplrs; i++)
    delete plplayers[i];
}

void NiceGame::terminate(bool natural) {
  setlastscore(score->pts(), lines, score->ppb(), player[0]->name().c_str(),
               bset);
  playing = false;
  if (timerid>=0)
    killTimer(timerid);
  emit quit();
}

void NiceGame::showbrick(FBPos const &pos, class PlPlayer *plp,
                       bool definitive) {
  vispit->addbrick(pos, plp!=plplayers[0], definitive);
}

void NiceGame::hidebrick(FBPos const & /*pos*/, class PlPlayer *plp) {
  vispit->rembrick(plp != plplayers[0]);
}

void NiceGame::setpause(bool onoff) {
  pause = onoff;
}


void NiceGame::start() {
  nextbox[0]->clear();
  if (nextbox[1])
    nextbox[1]->clear();
  logpit->clear();
  vispit->clear();
  statboard->setdata(SCORE, 0);
  statboard->setdata(LINES, lines = 0);
  statboard->setdata(LEVEL, player[0]->level(bset)); // should look at team lvl
  statboard->setdata(RANK, "-");
  statboard->setdata(PTSBLK, "-");

  nplrs = team ? 2 : 1;
  for (int i = 0; i < nplrs; i++)
    plplayers[i] = new PlPlayer(this, team ? (-1 + 2 * i) : pos,
                                *player[i],
                                sbd, global,
                                player[i]->level(bset), probabilities(bset),
                                team ? (i ? TEAMX1 : TEAMX0) : SOLOX0);
  for (int i = 0; i < nplrs; i++)
    plplayers[i]->connect(nextbox[i], logpit, team ? plplayers[1 - i] : 0);

  timerid = startTimer(1);
  playing = true;
}


void NiceGame::addscore(double sc, PlPlayer *) {
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
  //  sendreq(pudtime.next());
  pudlns = 0;
}

void NiceGame::timerEvent(QTimerEvent *) {
  if (pudding && pudtime.ivalgone()) {
    int i = GameTime::MID_PUD;
    pudtime.adjust(i);
    //sendreq(pudtime.getnext());
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
  for (int i = 0; i < nplrs; i++)
    plplayers[i]->poll();
  screenpit->poll();
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

void NiceGame::key(int code, bool in_not_out) {
  for (int i = 0; i < nplrs; i++)
    plplayers[i]->key(code, in_not_out);
}
  
