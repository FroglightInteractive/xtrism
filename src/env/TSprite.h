// TSprite.H

#ifndef _TSprite_H
#define _TSprite_H

#ifndef TPRIVATE
#include "TXID.h"
#endif

class TSprite {
public:
  TSprite(const class TGFX &tgfx, int w, int h);
  TSprite(const class TEnv &tenv, int w, int h);
  TSprite(const TSprite &other);
  TSprite(const class TImage &timage);
  ~TSprite();

  void read(const class TImage &timage);
  void read(const class TImage &timage,
              const class BBox &src, const class Point &dst);

  void tilerectangle(class BBox const & to) const;
  void put(int x, int y) const;
  void partput(class BBox const & from, class Point const & to) const;
  void setclip(class BBox const & bb);

  int width() const {
    return wid;
  }
  int height() const {
    return hei;
  }
#ifdef TPRIVATE
  Drawable drawable() const {
    return pixmap;
  }
#endif
private:
  // dev.indep
  class TGFX const & tgfx;
  int wid, hei;
private:
  // dev.dep
  void init();
#ifdef TPRIVATE
  Pixmap pixmap;
  GC gc;
#else
  TXID pixmap;
  TXID gc;
#endif
};

#endif