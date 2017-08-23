// TopBox.H - top level GBox: connects with TEnv

#ifndef _TopBox_H
#define _TopBox_H

#include "../env/TEnv.h"
#include "GBParent.h"
#include <list>
#include "../basics/Infty.h"
#include "../poll/PollServer.h"

class TopBox: public GBParent, private THKey, private THMML, private THRedraw,
  private THPoll, private THSync {
public:
  TopBox(TEnv &tenv, BBox const &bbox=BBox(0, 0, 0, 0),
         bool sharesel=false);
  virtual ~TopBox();
  void reenter(bool justleave=false, bool forgetstate=false);
  TopBox *topbox() {
    return this;
  }
  TGFX const &tgfx() {
    return env;
  }
public:
  PollServer poll, sync;
public:
  // for GBox
  void claim_mouse(GBox *b);
  void rel_mouse(GBox *b);
  // friend class GBox;
private:
  TEnv &env;
  bool containspointer;
  GBox *claimed_mouse;
private:
  bool inme(const Point &p);
  void tenterleave(bool enl);
  void tmotion(int x, int y);
  void tmbc(int button, bool in_not_out, int x, int y);
  void tredraw(int lx, int ty, int w, int h);
  void tkey(int kc, bool in_not_out);
  int tpoll(const class TTime &time) {
    return poll.poll(time);
  }
  bool tsync(class TTime const &time) {
    return sync.poll(time) == 0;
  }
};
#endif