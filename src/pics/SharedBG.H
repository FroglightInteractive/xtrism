// SharedBG.H

#ifndef _SharedBG_H
#define _SharedBG_H

#include "../basics/BBox.H"

class SharedBG {
public:
  SharedBG();
  virtual ~SharedBG();
  void draw(class TEnv const & env, class Filename const & imgf,
                BBox const & mybbox, bool usebytemap);
  void reg(class BGSharer *child);
  void redraw(BBox const &bbox) const;
  virtual class ByteMap *makebm(BBox const & /*bb*/) {
    return 0;
  }
  virtual bool draw(Point const & /*origin*/, class TImage */*img*/) {
    return 0;
  }
  virtual bool convert(class TImage */*img*/, class ByteMap */*bm*/) {
    return 0;
  }
private:
  BBox bb;
  class TImage *img;
  class ByteMap *bm;
  class BGSharer *first;
};

class BGSharer {
public:
  BGSharer(SharedBG &par);
  virtual ~BGSharer();
  void reg(class BGSharer *sib);
  BGSharer *next() {
    return nxt;
  }
  void redraw(BBox const &bb) const {
    par.redraw(bb);
  }
  virtual bool draw(Point const & /*origin*/, class ByteMap */*bm*/) {
    return 0;
  }
  virtual bool draw(Point const & /*origin*/, class TImage */*img*/) {
    return 0;
  }
private:
  BGSharer *nxt;
  SharedBG const &par;
};

#endif