// PlayButton.C

#include "PlayButton.h"

#include "../basics/dbx.h"
#include "../sessions/NiceSession.h"
#include "../options/PlayerList.h"
#include "../pics/MarbleBG.h"
#include "../env/TImage.h"

#include "../globals/Globals.h"

PlayButton::~PlayButton() {
  tenv().dereg_callback(this);
  dbx(-20070110, "~PlayButton");
}

void PlayButton::forcedloseselect() {
  dbx(1, "PB(%p): forcedloseselect", this);
  TextButton::forcedloseselect();
  visualselect(false);
}

bool PlayButton::offerselect() {
  dbx(1, "PB(%p): offserselect", this);
  visualselect(true);
  takeselect();
  return true;
}

void PlayButton::visualselect(bool sel) {
  string txt = gettext();
  if (txt.at(0) == '[')
    txt = txt.substr(1, txt.length() - 2);
  if (sel) {
    txt.insert(0, "[");
    txt.append("]");
  }
  settext(txt);
  parent()->redraw(bbox());
}

bool PlayButton::mouse(int b, bool io, const QPoint &p) {
  printf("PlayButton: Click! %i/%i (%i,%i) (this=%p)\n",
         b, io, p.x(), p.y(), this);
  if (io) {
    if (isselected())
      play();
    else {
      visualselect(true);
      takeselect();
    }
  }
  return true;
}

bool PlayButton::key(int kc, bool io) {
  if (!io)
    return false;

  switch (tenv().lastchar()) {
  case 13:
    if (isselected()) {
      play();
      return true;
    }
    break;

  case 's':
    if (!isselected()) {
      visualselect(true);
      takeselect();
    }
    break;

  case '1':
    if (isselected()) {
      play(0);
      return true;
    }
    break;

  case '2':
    if (isselected()) {
      play(1);
      return true;
    }
    break;

  case '3':
    if (isselected()) {
      play(2);
      return true;
    }
    break;

  case '4':
    if (isselected()) {
      play(3);
      return true;
    }
    break;
  }
  return false;
}

void PlayButton::redraw(BBox const &bbox) {
  dbx(2, "PlayButton(%p)::redraw - bgok=%i", this, bgok);
  if (!bgok) { // have to create our actual background.
    BBox const &mybb = this->bbox();
    int wid = bg.width(), hei = bg.height();
    marblebg(width(), height(), 2, // should be env.size dep.
             float(mybb.left()) / wid, float(mybb.top()) / hei,
             float(mybb.right()) / wid, float(mybb.bottom()) / hei,
             bg, mybb.left(), mybb.top());
    bgok = true;
    bg.partput(bbox, bbox.topleft());  // we can't be transp. the 1st time
  }
  TextButton::redraw(bbox);
}

void PlayButton::setsession(NiceSession *s0) {
  tthrow(s, "Already playing");
  tenv().reg_quit(this);
  s = s0;
}

void PlayButton::squit() {
  dbx(1, "PlayButton::squit");
  tenv().dereg_quit(this);
  tenv().reg_callback(this, 0);
  dbx(2, "PB:sq: done");
}

void PlayButton::callback(int code) {
  dbx(1, "PB: callback");
  if (s)
    delete s;
  s = 0;
}

void PlayButton::tquit() {
  dbx(1, "PB: tquit");
  if (s)
    delete s;
  s = 0;
}

void SoloButton::play(int bset) {
  if (bset >= 0)
    setbrickset(bset);
  dbx(1, "SoloButton::play");
  setsession(new NiceSession(bbox(), "1",
                             *player(right), brickset(), this));
  // start a game
}

void TeamButton::play(int bset) {
  dbx(1, "TeamButton::play");
  if (bset >= 0)
    setbrickset(bset);
  setsession(new NiceSession(bbox(), "2",
                             *player(false), *player(true),
                             brickset(), this));
  // start a game
}

void ApartButton::play(int bset) {
  dbx(1, "ApartButton::play");
  if (bset >= 0)
    setbrickset(bset);
  setsession(new NiceSession(bbox(), "3",
                             *player(false), *player(true),
                             brickset(), brickset(), this));
  // start a game
}