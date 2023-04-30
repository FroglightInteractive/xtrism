// PlPlayer.C

/* Sleeper info: must be warn()ed when logpit changes, and when kbdbuffer
   non-empty.
 * An alarm is set for godown. (NYI)
 */

#include <string>
#include "PlPlayer.h"

#include "Kronos.h"
#include "BrickEnv.h"
#include "FallBrick.h"
#include "Referee.h"
#include "KbdBuffer.h"
#include "WishList.h"
#include "Fortuna.h"
#include "NextBox.h"
#include "VisPit.h"
#include "LogPit.h"
#include "GameTime.h"
#include "NiceGame.h"

#include "../bricks/Probability.h"
#include "../options/Player.h"
#include "../options/GlobalOpts.h"
#include "../basics/dbx.h"
#include "../sound/Sounds.h"

const int AUTOREP_IVAL = 50;
const int AUTOREP_DELAY = 500;

PlPlayer::PlPlayer(NiceGame *g, int pos,
                   Player const &p,
                   SBrickData const &sbd, GlobalOpts const &glopts,
                   int lvl, Probability const &prob,
                   int x0, int y0, int rot0, int dy):
  game(g), player(p), state(PlPlayer::INIT),
  levup_autorep(0), levdn_autorep(0) {
  kronos = new Kronos(lvl);
  if (pos == 0 && !p.dwxpos())
    x0--;
  fallbrick = new FallBrick(sbd, x0, y0, rot0, dy);
  brickenv = 0;
  referee = new Referee(*kronos);
  kbdbuffer = new KbdBuffer(player.keys(pos), glopts.keys(pos));
  wishlist = new WishList(*fallbrick, *kronos);
  fortuna = new Fortuna(prob);
  nextbox = 0;
  other = 0;
}

void PlPlayer::connect(NextBox *nb, LogPit *lp, PlPlayer *oth) {
  other = oth;
  if (brickenv) {
    delete brickenv;
    brickenv = 0;
  }
  brickenv
    = other
    ? (BrickEnv *)new BrickEnv_Team(*lp, *other->fallbrick)
    : (BrickEnv *)new BrickEnv_Solo(*lp);
  fallbrick->setenv(brickenv);
  if (other)
    fortuna->regother(other->fortuna);
  nextbox = nb;

  fortuna->next();
  nextbno = fortuna->next();
  setstate(DELNEW);
}

PlPlayer::NBRes PlPlayer::newbrick() {
  switch (fallbrick->newbrick(nextbno)) {
  case BEC_SolidRock:
    return DIE;

  case BEC_Brick:
    return DELAY;

  case BEC_OK:
    queued_okland = false;
    queued_donepud = false;
    game->showbrick(fallbrick->position(), this);
    kronos->newbrick();
    nextbox->setbrick(nextbno=fortuna->next(), 0);
    return OK;
  }
  return OK; // shouldn't execute - just to avoid compiler warning
}

PlPlayer::~PlPlayer() {
  dbx(1, "~PlPlayer(%p)", this);
  delete fortuna;
  delete wishlist;
  delete kbdbuffer;
  delete referee;
  if (brickenv)
    delete brickenv;
  delete fallbrick;
  delete kronos;
  dbx(2, "~PlPlayer(%p) done", this);
  if (levup_autorep)
    delete levup_autorep;
  if (levdn_autorep)
    delete levdn_autorep;
}

void PlPlayer::disconnect(NextBox *nb) {
  dbx(1, "PlPlayer::disconnect(Nextbox=%p)", nb);
  nextbox = 0;
}

void PlPlayer::disconnect(PlPlayer *oth) {
  dbx(1, "PlPlayer::disconnect(Other=%p)", oth);
  other = 0;
}

void PlPlayer::poll() {
  switch (state) {
  case INIT:
    break;

  case PLAYING:
    poll_playing();
    break;

  case PAUSED:
  case OTHPAUSED:
    poll_pause();
    break;

  case DELLAND:
    break;

  case LANDED:
    break;

  case DELNEW:
    poll_delnew();
    break;

  case DEAD:
    break;
  }
}

void PlPlayer::poll_playing() {
  BufferCode bc = kbdbuffer->read();
  if (bc != BC_None) {
    wishlist->logkey(bc) || globalkey(bc);
  }

  WLResult wlr = wishlist->poll();
  if (wlr.moved) {
    game->showbrick(fallbrick->position(), this);
    if (wlr.othermoved)
      game->showbrick(other->fallbrick->position(), other);
  }
  //if (wlr.gonedown) {
  //  sendreq(kronos->nextdown());
  //  if (wlr.othermoved)
  //    other->sendreq(other->kronos->nextdown());
  //}
  if (wlr.landed)
    land(wlr.dropped);
}

void PlPlayer::poll_delnew() {
  switch (newbrick()) {
  case OK:
    setstate(PLAYING);
    break;

  case DELAY:
    break;

  case DIE:
    setstate(DEAD);
    game->terminate(true);
    break;
  }
}

void PlPlayer::poll_pause() { // !!!!!
  BufferCode bc = kbdbuffer->read();
  switch (bc) {
  case BC_iQuit:
    game->terminate(false);
    break;

  case BC_iPause:
    game->req_to_pause(this);
    break;

  case BC_iLevDn:
    game->req_to_changelev(-1, this);
    if (levdn_autorep)
      levdn_autorep->reset(AUTOREP_DELAY);
    else
      levdn_autorep = new GTimer(AUTOREP_DELAY);
    break;

  case BC_iLevUp:
    game->req_to_changelev(+1, this);
    if (levup_autorep)
      levup_autorep->reset(AUTOREP_DELAY);
    else
      levup_autorep = new GTimer(AUTOREP_DELAY);
    break;

  case BC_iLeft:
  case BC_iRight:
  case BC_iRotCW:
  case BC_iRotCCW:
  case BC_iDrop:
  case BC_iZLeft:
  case BC_iZRight:
  case BC_iZap:
    if (game->req_to_unpause(this))
      kbdbuffer->putback(bc);
    break;

  case BC_oLevDn:
    if (levdn_autorep)
      delete levdn_autorep;
    levdn_autorep = 0;
    break;

  case BC_oLevUp:
    if (levup_autorep)
      delete levup_autorep;
    levup_autorep = 0;
    break;

  default:
    if (levdn_autorep && levdn_autorep->ivalgone()) {
      levdn_autorep->reset(AUTOREP_IVAL);
      game->req_to_changelev(-1, this);
    }
    if (levup_autorep && levup_autorep->ivalgone()) {
      levup_autorep->reset(AUTOREP_IVAL);
      game->req_to_changelev(+1, this);
    }
    break;
  }
  // update visual pause, if owner
}

bool PlPlayer::globalkey(BufferCode &bc) { // !!!!!
  dbx(2, "PlPlayer::globalkey");
  switch (bc) {
  case BC_iQuit:
    game->terminate(false);
    return true;

  case BC_iPause:
    game->req_to_pause(this);
    return true;

  default:
    return false;
  }
}

void PlPlayer::land(bool dropped=0) { // !!!!!
  sounds->drop();
  dbx(1, "PlPlayer(%p)::land", this);
  setstate(DELLAND);
  referee->brickprepare(fallbrick->position().y, dropped);
  game->req_to_land(this);
}

void PlPlayer::ok_to_land() {
  if (state == OTHPAUSED || state == PAUSED) {
    queued_okland = true;
    return;
  }
  if (state != DELLAND)
    throw "PlPlayer: Unexpected ok_to_land";
  queued_okland = false;
  dbx(1, "PlPlayer(%p)::ok_to_land", this);
  setstate(LANDED);
  fallbrick->drop();
  yland = fallbrick->position().y;
  game->bricklanded(fallbrick->position(), this);
}

void PlPlayer::done_pudding(int lns) {
  if (lns >= 0) // -1 <=> delayed reaction
    game->addscore(referee->linesscore(lns)
                   + referee->brickscore(yland), this);
  if (state == PAUSED || state == OTHPAUSED) {
    queued_donepud = true;
    return;
  }
  if (!queued_donepud && state != LANDED)
    throw "PlPlayer: Unexpected done_pudding";
  queued_donepud = false;
  dbx(1, "PlPlayer(%p)::done_pudding", this);
  setstate(DELNEW);
}

void PlPlayer::notifypudding(int y, int dir) {
  if (state == LANDED || queued_okland || queued_donepud)
    return; // no action if I'm not in the air

  fallbrick->notifypudding(y, dir);
}

void PlPlayer::pause(bool own) {
  dbx(1, "PlPlayer(%p)::pause own = %p", this, own);
  if (state != PAUSED && state != OTHPAUSED)
    kronos->markpause(true);
  if (own) {
    if (state == OTHPAUSED) { // steal pause
      dbx(1, "PlPlayer(%p) steals pause", this);
    } else if (state != PAUSED) { // visual pause
      dbx(1, "PlPlayer(%p) takes new pause", this);
    }
    // else athrow("PlPlayer: Unexpected pause");
  } else if (state == PAUSED) { // lose pause
    dbx(1, "PlPlayer(%p) yields pause", this);
  }
  setstate(own ? PAUSED : OTHPAUSED);
}

void PlPlayer::unpause(bool payforit) {
  if (state == PAUSED) { // visual unpause
    dbx(1, "PlPlayer(%p) unpauses (used to own!)");
  } else {
    dbx(1, "PlPlayer(%p) unpauses (didn't own)");
    if (state != OTHPAUSED)
      throw "PlPlayer: Unexpected unpause";
  }
  kronos->markpause(false, payforit);
  setstate(PLAYING);
  if (queued_okland)
    ok_to_land();
  if (queued_donepud)
    done_pudding();
}

void PlPlayer::setstate(State s) {
  state = s;
  dbx(1, "PlPlayer(%p)::state = %i", this, int(s));
}

void PlPlayer::key(int code, bool in_not_out) {
  kbdbuffer->enter(code, in_not_out);
}
