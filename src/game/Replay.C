// Replay.C

// code to replay a game of trism from a Logger file

#include "Replay.H"
#include "Logger.H"
#include "LoggerP.H"

#include "../basics/Throw.H"
#include "NextBox.H"
#include "VisPit.H"

Replay::Replay(Logger *logger, VisPit *vp0,
               NextBox *nb0, NextBox *nb1):
  nextoffset(0),
  data(logger->givedata()),
  nexttime(TTime::INFINITE_PAST) {
  vp=vp0; nb[0]=nb0; nb[1]=nb1;
  }

int Replay::step(TTime const &t) {
  if (t<nexttime)
    return 0;
  LEvent &lev=data->event(nextoffset);
  switch (lev.header.type)
    {
    case LT_ADDBRICK:
      vp->addbrick(FBPos(lev.addrembrick.bno, lev.addrembrick.rot,
                         lev.addrembrick.x, lev.addrembrick.y),
                   lev.addrembrick.id, lev.addrembrick.def);
      break;
    case LT_REMBRICK:
      if (lev.addrembrick.id<0)
        vp->rembrick(FBPos(lev.addrembrick.bno, lev.addrembrick.rot,
                         lev.addrembrick.x, lev.addrembrick.y));
      else
        vp->rembrick(lev.addrembrick.id);
      break;
    case LT_PUDDING:
      vp->pudding(lev.pudding.y,-1+2*lev.pudding.dir);
      break;
    case LT_NEWNEXT: // or case LT_QUIT:
      if (lev.newnext.bno==63)
        { // LT_QUIT
          nexttime=TTime::INFINITE_FUTURE;
          return -1; // mark ended
        }
      else
        { // real LT_NEWNEXT
          nb[lev.newnext.id]->update(lev.newnext.bno,lev.newnext.rot);
        }
      break;
    }
  // if not quit, prepare for next event:
  nexttime+=Logger_invtimerep(data->event(++nextoffset).header.time);
  return 1;
  }
