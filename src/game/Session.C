// Session.C

#include "Session.H"
#include "GameQuit.H"
#include "Game.H"
#include "../basics/dbx.H"
#include "../basics/Filename.H"
#include "../bricks/brickcell.H"
#include "../gfx/GBParent.H"
#include "../gfx/GBox.H"
#include "../env/TImage.H"
#include "../pics/MarbleBG.H"

Session::Session(TEnv &tenv0, class GBParent *gbp0,
                 GameQuit *gq, BBox const &bgbbox):
  qquitable(false), gquit(gq), gbp(gbp0), defunct(false), tenv(tenv0) {
  dbx(1,"Session::Session");
  l=float(bgbbox.left())/tenv.width();
  t=float(bgbbox.top())/tenv.height();
  r=float(bgbbox.right())/tenv.width();
  b=float(bgbbox.bottom())/tenv.height();
  tenv.hideCursor();
  }

Session::~Session() {
  dbx(1,"Session::~Session");
  tenv.restoreCursor();
  }


class ByteMap *Session::makebm(BBox const &bb) {
  return new BrickCell(bb.width(),bb.height());
  // perhaps draw some nice trees in later vsn?
  }

bool Session::convert(class TImage *img, class ByteMap *bm) {
  marblebg(gbp->width(),gbp->height(),0,
           l,t,r,b,
           *img, 0,0,
           bm);
  return true;
  }


void Session::redraw(BBox const &bb) {
  if (!defunct)
    SharedBG::redraw(bb);
  }

void Session::reg_game(class Game *game) {
  games.push_back(game);
  qquitable=false;
  }

void Session::dereg_game(class Game *game) {
  quit_game(game,false);
  for (vector<Game *>::iterator i=games.begin();
       i!=games.end(); ++i)
    if (*i == game)
      { games.erase(i); return; }
  }

void Session::quit_game(class Game *game, bool natural) {
  bool qq=true;
  bool wasp=false;
  for (vector<Game *>::iterator i=games.begin();
       i!=games.end(); ++i)
    if (*i == game && (*i)->isplaying())
      { (*i)->quit(natural); wasp=true; }
    else
      qq&=!(*i)->isplaying();
  qquitable=qq;
  if (wasp && gquit)
    gquit->gquit(game,qquitable);
  dbx(1,"Session::quit_game done");
  }

void Session::start_game(class Game *game) {
  qquitable=false;
  for (vector<Game *>::iterator i=games.begin();
       i!=games.end(); ++i)
    if (*i == game)
      (*i)->start();
  }

void Session::draw(class TEnv const &env, char const *dir, char const *id) {
  SharedBG::draw(env,(Filename(dir)+(string("bg")+id)).addpart(env.id()),
                 gbp->bbox(),true);
  redraw(gbp->bbox());
  }
