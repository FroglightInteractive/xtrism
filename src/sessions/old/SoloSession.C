// SoloSession.C

#include "SoloSession.H"
#include "../globals/Globals.H"
#include "SessionQuit.H"

SoloSession::SoloSession(Player const &p, int bset, SessionQuit *sq):
  Session(tenv(),this),
  squit(sq) {
  dbx(1,"SoloSession");
  sg=new SoloGame(*this, this, 0,
                  keyboard(),
                  p,globalopts(),
                  sbd(), bs(),
                  bset, probbset(),
                  tf(), tfyellow());
  newchild(sg,GBPos(this,1),GBPos(this,1),GBPos(this,1),GBPos(this,1));
  placechildren();
  draw();
  dbx(2,"SS: starting game");
  start_game(sg);
  dbx(3,"SS: .. done");
  }

SoloSession::~SoloSession() {
  dbx(1,"~SoloSession");
  if (sg)
    delete sg;
  }

void SoloSession::gquit(class GameForSession *g, bool quitable) { 
  dbx(1,"SS(): qquit");
  if (g!=sg)
    athrow("SoloSession::gquit: unexpected Game *");
  if (squit)
    squit->squit();
  }
