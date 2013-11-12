// SoloSession.H

#ifndef _SoloSession_H
#define _SoloSession_H

#include "../game/Session.H"
#include "../game/SoloGame.H"
#include "../game/GameQuit.H"

class SoloSession: public Session, private GameQuit {
  public:
    SoloSession(Player const &p, int bset, class SessionQuit *sq=0);
    ~SoloSession();
  private:
    SoloGame *sg;
    void gquit(class GameForSession *g, bool quitable);
    class SessionQuit *squit;
  };
#endif
