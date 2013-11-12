// GameInit.C

#include <string>

#include "SoloGame.H"
#include "PlPlForGame.H"
#include "PlPlayer.H"
#include "LogPit.H"
#include "Score.H"
#include "Ranker.H"
#include "Keyboard.H"
#include "VisPit.H"
#include "ScreenPit.H"
#include "NextBox.H"
#include "StatBoard.H"
#include "GameTime.H"
#include "SessionForGame.H"
#include "../bricks/Probability.H"
#include "../env/TFont.H"
#include "../options/Player.H"
#include "../options/GlobalOpts.H"
#include "../bricks/data.H"
#include "../bricks/bsprites.H"
#include "../basics/dbx.H"

const int SOLOWIDTH=10;
const int SOLOHEIGHT=22;
const int SOLOX0=8;

// this vsn can't even do 2p2p, so ignores the pos field

SoloGame::SoloGame(SessionForGame &s, GBParent *gbp, int pos0,
                   Keyboard &kbd,
                   Player const &p, GlobalOpts const &g,
                   SBrickData const &sbd0, BrickSprites const &bs0,
                   int bset, ProbBSet const &pbs0,
                   TFont const &labelf, TFont const &dataf):
  GBParent(gbp, gbp->width(),gbp->height()),
  Game(s,kbd,g,sbd0,bs0,bset,pbs0),
  player(p), pos(pos0) {
  dbx(1,"SoloGame %p",this);
  int lw=labelf.stringwidth("Pts/Blk:");
  int dw=dataf.stringwidth("Heroic!");
  StatBoard *sb=new StatBoard(this, NLINES, lw, dw, labelf, dataf);
  VisPit *vp=new VisPit(SOLOWIDTH,SOLOHEIGHT, sbd0, bs0);
  ScreenPit *sp=new ScreenPit(this, *vp,0,0);
  Game::connect(sb,sp,vp);
  
  nextbox=new NextBox(this, sbd0, bs0);
  tdc=new TDrawCol(gbp->tgfx(), TRGB(170,0,0));

  statboard->setlabel(SCORE,"Score:",false);
  statboard->setlabel(LINES,"Lines:",false);
  statboard->setlabel(LEVEL,"Level:",false);
  statboard->setlabel(RANK,"Rank:",false);
  statboard->setlabel(PTSBLK,"Pts/Blk:",false);

  newchild(sb,GBPos(this,1),GBPos(this,height()/10),GBPos(sp,1),0);
  newchild(sp,GBPos(sb,1),0,GBPos(nextbox,1),GBPos(this,height()/10));
  newchild(nextbox,GBPos(sp,1),0,GBPos(this,1),GBPos(sp));
  placechildren();
  }

SoloGame::~SoloGame() {
  dbx(2,"~SoloGame %p",this);
  if (playing)
    { Game::quit(0); }
  if (nplrs)
    plplayers[0]->disconnect(nextbox);
  delete nextbox;
  delete tdc;
  }

void SoloGame::start() {
  LogPit *lp=new LogPit(SOLOWIDTH,SOLOHEIGHT,sbd);
  nextbox->clear();
  vispit->clear();
  statboard->setdata(SCORE,0);
  statboard->setdata(LINES,lines=0);
  statboard->setdata(LEVEL,player.level(bset));
  statboard->setdata(RANK,"-");
  statboard->setdata(PTSBLK,"-");
  Ranker *rk=new Ranker(); // incomplete...

  Game::connect(lp,rk);
  
  plplayers[0]=new PlPlayer((GameForPlPl*)(this), pos,
                            player,
                            sbd,global,&keyboard,
                            player.level(bset), pbs[bset],
                            SOLOX0);
  plplayers[0]->connect(nextbox,*logpit);
  nplrs=1;
  
  GBox::reg_poll();
  Game::start();
  }

extern void setlastscore(int sc, int li, double ppb);

void SoloGame::quit(bool natural) {
  setlastscore(score->pts(),lines,score->ppb());
  Game::quit(natural);
  GBox::dereg_poll();
  }

void SoloGame::addscore(double sc, PlPlForGame *plp) {
  tthrow(plp!=plplayers[0], "SoloGame::addscore: Unexpected PlPlayer");
  (*score)+=sc;
  dbx(1,"SoloGame::addscore %g",sc);
  statboard->setdata(SCORE,score->pts());
  statboard->setdata(PTSBLK,score->ppb());
  }

void SoloGame::req_to_land(PlPlForGame *plp) {
  landreq=plp;
  }

void SoloGame::bricklanded(FBPos const &pos, PlPlForGame *plp) {
  tthrow(plp!=plplayers[0], "SoloGame::bricklanded: Unexpected PlPlayer");
  showbrick(pos,plp,true);
  logpit->or(pos.bno, pos.rot, pos.x, pos.y);
  puddreq=plp;
  pudding=1;
  int i=GameTime::PRE_PUD; pudtime.reset(i);
  pudlns=0;
  }

void SoloGame::poll(TTime const &t) {
  Game::poll();
  if (nplrs)
    plplayers[0]->poll();

  if (pudding && pudtime.ivalgone())
    { int i=GameTime::MID_PUD-GameTime::PRE_PUD;
      pudtime.adjust(i);
      int y=logpit->findfullline();
      dbx(1,"Game:logpitpoll: y=%i",y);
      if (y>=0)
        { logpit->collapseline(y,pudding);
          vispit->pudding(y,pudding);
          plplayers[0]->updlevel(++lines);
          statboard->setdata(LINES,lines);
          statboard->setdata(LEVEL,plplayers[0]->getlevel());
          pudlns++;
        }
      else
        { pudding=0;
          if (puddreq)
            { puddreq->done_pudding(pudlns); puddreq=0; }
        }
    }
  if (!pudding && landreq)
    { landreq->ok_to_land(); landreq=0; }
  }

void SoloGame::req_to_changelev(int change, PlPlForGame *plp) {
  // check whether allowed
  int nlev=plp->getlevel()+change;
  plp->setlevel(nlev);
  statboard->setdata(LEVEL,nlev);
  }

bool SoloGame::req_to_pause(PlPlForGame *plp) {
  plp->pause(true);
  pause=true;
  return true;
  }

bool SoloGame::req_to_unpause(PlPlForGame *plp) {
  plp->unpause(false);
  pause=false;
  return true;
  }

bool SoloGame::req_to_quit(bool dead, PlPlForGame *plp) {
  selfquit(dead);
  return true;
  }

void SoloGame::redraw(BBox const &bb) {
  dbx(1,"SoloGame::redraw bb=(%i,%i)-(%i,%i)",bb.left(),bb.top(),
      bb.right(),bb.bottom());
  tdc->rectangle(bb);
  GBParent::redraw(bb);
  }
