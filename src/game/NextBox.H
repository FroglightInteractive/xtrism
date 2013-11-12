// NextBox.H

#ifndef _NextBox_H
#define _NextBox_H

#include "../gfx/GBox.H"
#include "../pics/SharedBG.H"
#include "../poll/Sleeper.H"

class NextBox: public GBox, public BGSharer, public Sleeper {
  public:
    NextBox(class GBParent *p, class SBrickData const &sbd0,
            class BrickSprites const &bs0,
            SharedBG &sbg, class PollServer &syncserv,
            bool myid=0, class Logger *logger=0);
    ~NextBox();
    bool draw(Point const &origin, class ByteMap *bm);
    void redraw(BBox const &bbox);
    void update(int bno0, int rot0);
    void poll();
    void clear() { update(-1,0); }
  private:
    class SBrickData const &sbd;
    class BrickSprites const &bs;
    int bno, rot;
    bool myid;
    class Logger *logger;
  };

#endif
