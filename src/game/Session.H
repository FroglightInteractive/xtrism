// Session.H

#ifndef _Session_H
#define _Session_H

#include "../env/TEnv.H"
#include <vector>
#include "Keyboard.H"
#include "../pics/SharedBG.H"

class Session: public SharedBG {
  public:
    Session(TEnv &tenv, class GBParent *gbp,
            class GameQuit *gq, BBox const &bgbbox);
    virtual ~Session();
    virtual void redraw(BBox const &bb);
    void quit_game(class Game *game, bool naturalp); // must NOT be called while threaded
    void start_game(class Game *game);
    Keyboard &keyboard() { return kbd; }
    void setdefunct() { defunct=true; }
    class GBParent &gbpar() { return *gbp; }
    class TEnv &env() { return tenv; }
  public: // from SessionForGame // for Game ONLY
    void reg_game(class Game *game); // must NOT be called while threaded
    void dereg_game(class Game *game);
  private: // from SharedBG
    class ByteMap *makebm(BBox const &bb);
    bool convert(class TImage *img, class ByteMap *bm);
  public: // for NiceSession (etc) ONLY
    void draw(class TEnv const &env, char const *dir, char const *id);
  private:
    vector<class Game *> games;
    bool qquitable;
    class GameQuit *gquit;
//    class TopBox *tb;
    class GBParent *gbp;
    Keyboard kbd;
    bool defunct;
    float l,t,r,b;
    class TEnv &tenv;
  };

#endif
