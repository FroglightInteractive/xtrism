// BoxMarbler.H

#ifndef _BoxMarbler_H
#define _BoxMarbler_H

#include "../env/TCmap.h"
#include <list>

class BoxMarblers {
public:
  BoxMarblers(class TEnv const &tenv,
                unsigned int tilew, unsigned int tileh,
                unsigned int maxlen,
                unsigned int depth=128);
  ~BoxMarblers();
  class BoxMarbler *find(TRGB const &rgb);
private:
  inline unsigned char readpoint(int x, int y) const;
private:
  class ByteMap *tile;
  class TImage *horiborder, *vertborder;
  list<BoxMarbler *> boxes;
  class TEnv const & tenv;
  friend class BoxMarbler;
};

class BoxMarbler {
public:
  BoxMarbler(class TRGB const &rgb, BoxMarblers *par);
  ~BoxMarbler();
  void draw(class BBox const & mybbox, class BBox const & redrawbbox,
                int raise, int borderw); // raise = +/- 1.
  bool operator==(TRGB const &rgb1) const {
    return rgb1 == rgb;
  }
private:
  TRGB rgb;
  class TImage *img;
  BoxMarblers *parent;
  friend class BoxMarblers;
};

#endif
