// NiceSession.C

#include "NiceSession.h"
#include "../globals/Globals.h"
#include "SessionQuit.h"
#include "../game/NiceGame.h"
#include "../game/Session.h"
#include "../basics/Filename.h"
#include "../env/TImageFile.h"
#include "../sound/Sounds.h"

NiceSession::NiceSession(BBox const &bb, char const *id,
                         Player const &p1, int bset, class SessionQuit *sq):
  TopBox(tenv(), BBox(0, 0, tenv().width(), tenv().height()), true),
  squit(sq) {
  dbx(1, "NiceSession: Solo");
  init(bb);
  g1 = new NiceGame(*s, 0,
                    poll, sync,
                    s->keyboard(),
                    &p1, 0, globalopts(),
                    sbd(), bs(), bs(),
                    bset, probbset(),
                    tf(), tfyellow());
  g2 = 0;
  start(id);
}

NiceSession::NiceSession(BBox const &bb, char const *id,
                         Player const &p1, Player const &p2,
                         int bset, class SessionQuit *sq):
  TopBox(tenv(), BBox(0, 0, tenv().width(), tenv().height()), true),
  squit(sq) {
  dbx(1, "NiceSession: Team");
  init(bb);
  g1 = new NiceGame(*s, 0, poll, sync,
                    s->keyboard(),
                    &p1, &p2, globalopts(),
                    sbd(), bs(), bs2(),
                    bset, probbset(),
                    tf(), tfyellow());
  g2 = 0;
  start(id);
}

NiceSession::NiceSession(BBox const &bb, char const *id,
                         Player const &p1, Player const &p2,
                         int bset1, int bset2, class SessionQuit *sq):
  TopBox(tenv(), BBox(0, 0, tenv().width(), tenv().height()), true),
  squit(sq) {
  dbx(1, "NiceSession: Apart");
  init(bb);
  g1 = new NiceGame(*s, -1, poll, sync,
                    s->keyboard(),
                    &p1, 0, globalopts(),
                    sbd(), bs(), bs(),
                    bset1, probbset(),
                    tf(), tfyellow());
  g2 = new NiceGame(*s, 1, poll, sync,
                    s->keyboard(),
                    &p2, 0, globalopts(),
                    sbd(), bs(), bs(),
                    bset2, probbset(),
                    tf(), tfyellow());

  // This placement stuff is untested...
  newchild(g1, GBPos(this, 0), 0, 0, GBPos(this, 0));
  newchild(g2, 0, 0, GBPos(this, 0), GBPos(this, 0));
  placechildren();

  start(id);
}

void NiceSession::init(BBox const &bb) {
  s = new Session(tenv(), this, this, bb);
}

void NiceSession::start(char const *id) {
  newchild(g1, GBPos(this, 1), GBPos(this, 1),
           GBPos(g2 ? (GBParent *)g2 : (GBParent *)this, 1), GBPos(this, 1));
  if (g2)
    newchild(g2, GBPos(g1, 1), GBPos(this, 1), GBPos(this, 1),
             GBPos(this, 1));
  s->draw(tenv(), cachedir().name(), id);
  dbx(2, "NS: starting game");
  s->start_game(g1);
  if (g2)
    s->start_game(g2);
  dbx(3, "NS: game(s) should be running");
}

void NiceSession::redraw(BBox const &bb) {
  s->redraw(bb);
  TopBox::redraw(bb);
}

NiceSession::~NiceSession() {
  dbx(1, "~NiceSession");
  if (g2)
    delete g2;
  if (g1)
    delete g1;
  delete s;
}

void NiceSession::gquit(class Game *g, bool quitable) {
  dbx(1, "NS: gquit");
  // should we do after game statistics here?
  fthrow(g == g1 || g == g2, "NiceSession::gquit: Unknown game");
  if (quitable) {
    if (squit)
      squit->squit();
  } else { // one game running, one game ended...
           // perhaps do g->prepare_to_play_again() ?
  }
}

bool NiceSession::key(int code, bool in_not_out) {
  return s->keyboard().key(code, in_not_out);
}
