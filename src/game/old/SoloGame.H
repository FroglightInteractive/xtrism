// SoloGame.H

#ifndef _SoloGame_H
#define _SoloGame_H

#include "Game.H"

#include "../gfx/GBParent.H"

class SoloGame: public GBParent, public Game {
  public:
    SoloGame(class SessionForGame &s, class GBParent *gbp, int pos0,
             class Keyboard &kbd,
             class Player const &p, class GlobalOpts const &g,
             class SBrickData const &sbd0, class BrickSprites const &bs0,
             int bset, class ProbBSet const &pbs0,
             class TFont const &labelf, class TFont const &dataf);
    virtual ~SoloGame();
    void start();
    void quit(bool natural);
    void poll(const class TTime &t);
    virtual void redraw(BBox const &bb);
  private: // from GameForPlPl
    void addscore(double sc, class PlPlForGame *plp);
    void req_to_land(class PlPlForGame *plp);
    void bricklanded(class FBPos const &pos, class PlPlForGame *plp);
    void req_to_changelev(int change, class PlPlForGame *plp);
    bool req_to_pause(class PlPlForGame *plp);
    bool req_to_unpause(class PlPlForGame *plp);
    bool req_to_quit(bool dead, class PlPlForGame *plp);
  private:
    enum StatLines {
      SCORE=0, LINES, LEVEL, RANK, PTSBLK, NLINES,
      };
    class Player const &player;
    class NextBox *nextbox;
    int lines;
    int pos;
    class TDrawCol *tdc;
  };

#endif
