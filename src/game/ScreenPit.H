// ScreenPit.H

#ifndef _ScreenPit_H
#define _ScreenPit_H

#include "../gfx/GBox.H"
#include "VisPit.H"
#include "../pics/SharedBG.H"
#include "../poll/Sleeper.H"

class ScreenPit: public GBox, public BGSharer, public Sleeper {
  public:
    ScreenPit(GBParent *p, VisPit &vp, SharedBG &sbg,
              class PollServer &syncserv);
    /* In the GBArgs argument, the width and height members need not
       be specified; they are re-calculated from the VisPit's data. */
    ~ScreenPit();
    bool draw(Point const &origin, class ByteMap *bm);
    void redraw(BBox const &bbox);
    void poll(); // for synchronized drawing
    void redrawcell(int x, int y, TSprite const *tsp) const;
  private:
    int cellsize() const { return vispit.cellsize(); }
    VisPit &vispit;
    class TImage *backg;
    class GBox const *backg_origin;
    bool bgok;
  };

#endif
