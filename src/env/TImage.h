// TImage.H

#ifndef _TImage_H
#define _TImage_H

/* For reasons of speed, this header uses inline functions for setting
   and reading pixels (only). This means that we have to explicitly
   include Xlib.h. When porting, look for ** marked lines.
 */
/* Note that TImage::width(), height() and depth() are not inline unlike
   for most Trism classes. Don't use them inside loops!
 */

// #ifdef TPRIVATE
#include <X11/Xlib.h>
// #else
// #include "TXID.h"
// #endif

#include "TCmap.h"

class TImage {
public:
  // dev.indep.
  TImage(const class TEnv &tenv, unsigned int w, unsigned int h);
  ~TImage();
  unsigned int width() const;
  unsigned int height() const;
  unsigned int depth() const;
  void putpix(int x, int y, TCol col) { // ** should not be in header file...
    image->f.put_pixel(image, x, y, (unsigned int)col);
  }
  TCol getpix(int x, int y)  // ** should not be in header file...
  const {
    return TCol(image->f.get_pixel(image, x, y));
  }
  const class TEnv &tenv() const {
    return env;
  }
  void put(int x, int y) const;
  void partput(class BBox const & from, class Point const & to) const;
  void tile(class BBox const & dest) const;
protected:
  // dev.dep.
#ifdef TPRIVATE
  TImage(const class TEnv &tenv, XImage *xim, bool delafter=0);
  char *data() const {
    return image->data;
  }
  unsigned int linelength() const {
    return image->bytes_per_line;
  }
  XImage *ximage() const {
    return image;
  }
#endif
  friend class TImageFile;
  friend class TSprite;
private:
  // dev.dep.
  const class TEnv &env;
// #ifdef TPRIVATE
  XImage *image;
  bool priv;
// #else
// void *image;
// bool priv;
// #endif
};

#endif