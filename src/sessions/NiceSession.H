// Solo, Team, Apart sessions

#ifndef _NiceSession_H
#define _NiceSession_H

#include "../game/GameQuit.H"
#include "../gfx/TopBox.H"

class NiceSession: public TopBox, private GameQuit {
  public:
    NiceSession(class BBox const &bb, char const *id,
                class Player const &p1, int bset,
                class SessionQuit *sq=0); // Solo
    NiceSession(class BBox const &bb, char const *id,
                class Player const &p1, class Player const &p2,
                int bset, class SessionQuit *sq=0); // Team
    NiceSession(class BBox const &bb, char const *id,
                class Player const &p1, class Player const &p2,
                int bset1, int bset2, class SessionQuit *sq=0); // Apart
    virtual ~NiceSession();
    bool key(int code, bool in_not_out);
    void redraw(BBox const &bb);
  private:
    void init(class BBox const &bb);
    void start(char const *id);
    void gquit(class Game *g, bool quitable);
  private:
    class Session *s;
    class NiceGame *g1, *g2;
    class SessionQuit *squit;
  };

#endif
