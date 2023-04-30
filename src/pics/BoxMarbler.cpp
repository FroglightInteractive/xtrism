// BoxMarbler.C

#include "../env/TImage.h"
#include "../env/TEnv.h"
#include "../bytemap/ByteMap.h"
#include "../bricks/brickcell.h"
#include "../basics/BBox.h"
#include "../bytemap/ByteImage.h"
#include "../bytemap/WideRGB.h"

#include "BoxMarbler.h"
#include "../basics/dbx.h"

const int TOPRAISE = 64;
const int LEFTRAISE = 48;
const int RIGHTRAISE = -48;
const int BOTTOMRAISE = -64;

BoxMarblers::BoxMarblers(class TEnv const &tenv0,
                           unsigned int tilew, unsigned int tileh,
                           unsigned int maxlen,
                           unsigned int depth):
  tile(new ByteMap(tilew, tileh)),
  horiborder(new TImage(tenv0, maxlen, 1)),
  vertborder(new TImage(tenv0, 1, maxlen)),
  tenv(tenv0) {
  dbx(-20070110, "BoxMarblers(%p)", this);
  BrickCell bc(2 * tilew, 2 * tileh, depth);
  for (unsigned int y = 0; y < tileh; y++) {
    float Y = float(y) / tileh;
    for (unsigned int x = 0; x < tilew; x++) {
      float X = float(x) / tilew;
      tile->c(x, y)
        = (unsigned char)(Y * (X * bc.cc(x, y)
                               + (1-X) * bc.cc(x + tilew, y))
                          + (1-Y) * (X * bc.cc(x, y + tileh)
                                       + (1-X) * bc.cc(x + tilew, y + tileh)));
    }
  }
}

BoxMarblers::~BoxMarblers() {
  dbx(-20070110, "~BoxMarblers(%p)", this);
  for (list<BoxMarbler *>::iterator i = boxes.begin(); i != boxes.end(); ++i)
    delete (*i);
  delete horiborder;
  delete vertborder;
  delete tile;
}

BoxMarbler *BoxMarblers::find(TRGB const &rgb) {
  for (list<BoxMarbler *>::iterator i = boxes.begin(); i != boxes.end(); ++i)
    if (*(*i) == rgb)
      return *i;

  // so we haven't found a match
  BoxMarbler *nw = new BoxMarbler(rgb, this);
  boxes.push_back(nw);
  return nw;
}

inline unsigned char BoxMarblers::readpoint(int x, int y) const {
  return tile->cc(x % tile->width(), y % tile->height());
}

///////////////////////////////// BoxMarbler /////////////////////////////////

BoxMarbler::BoxMarbler(TRGB const &rgb0, BoxMarblers *par):
  rgb(rgb0),
  img(new TImage(par->tenv,
                 par->tile->width(),
                 par->tile->height())),
  parent(par) {
  dbx(-20070110, "BoxMarbler(%p)", this);
  WideRGB rgb0a(rgb);
  rgb0a -= 100;
  WideRGB rgb1(rgb);
  rgb1 += 100;
  byteimage(*img, *(par->tile), rgb0a, rgb1);
}

BoxMarbler::~BoxMarbler() {
  dbx(-20070110, "~BoxMarbler(%p)", this);
  delete img;
}

void BoxMarbler::draw(BBox const &mybbox, BBox const &redrawbbox,
                      int raise, int borderw) {
  // interior
  dbx(-980820, "BoxMarbler: my=(%i,%i)-(%i,%i) draw=(%i,%i)-(%i,%i)",
      mybbox.left(), mybbox.top(), mybbox.right(), mybbox.bottom(),
      redrawbbox.left(), redrawbbox.top(), redrawbbox.right(),
      redrawbbox.bottom());
  if (!(mybbox & redrawbbox))
    dbx(-980820, "   No intersection");
  BBox interior(mybbox);
  interior.addmargin(-borderw);
  interior &= redrawbbox;
  img->tile(interior);

  int width = mybbox.width();
  int height = mybbox.height();
  int left = mybbox.left();
  int top = mybbox.top();
  int right = mybbox.right();
  int bottom = mybbox.bottom();

  TCmap const &cm(parent->tenv.cmap());

  // top edge
  for (int y = 0; y < borderw; y++) {
    for (int x = y; x < width - y - 1; x++) {
      int p = parent->readpoint(x + left, y + top) - 128 + TOPRAISE * raise;
      TRGB t(rgb);
      t += p;
      parent->horiborder->putpix(left + x, 0, cm(t));
    }
    BBox bb(left + y, top + y, right - 1 - y, top + y + 1);
    bb &= redrawbbox;
    if (bb) {
      QPoint pp(bb.topleft());
      bb.shift(0, -top - y);
      parent->horiborder->partput(bb, pp);
    }
  }

  // bottom edge
  for (int y = 0; y < borderw; y++) {
    for (int x = y + 1; x < width - y; x++) {
      int p
        = parent->readpoint(x + left,
                            bottom - 1 - y) - 128 + BOTTOMRAISE * raise;
      TRGB t(rgb);
      t += p;
      parent->horiborder->putpix(left + x, 0, cm(t));
    }
    BBox bb(left + y + 1, bottom - 1 - y, right - y, bottom - y);
    bb &= redrawbbox;
    if (bb) {
      QPoint pp(bb.topleft());
      bb.shift(0, -bottom + 1 + y);
      parent->horiborder->partput(bb, pp);
    }
  }

  // left edge
  for (int x = 0; x < borderw; x++) {
    for (int y = x + 1; y < height - x; y++) {
      int p = parent->readpoint(x + left, top + y) - 128 + LEFTRAISE * raise;
      TRGB t(rgb);
      t += p;
      parent->vertborder->putpix(0, top + y, cm(t));
    }
    BBox bb(left + x, top + x + 1, left + x + 1, bottom - x);
    bb &= redrawbbox;
    if (bb) {
      QPoint pp(bb.topleft());
      bb.shift(-left - x, 0);
      parent->vertborder->partput(bb, pp);
    }
  }

  // right edge
  for (int x = 0; x < borderw; x++) {
    for (int y = x; y < height - 1 - x; y++) {
      int p
        = parent->readpoint(right - 1 - x,
                            top + y) - 128 + RIGHTRAISE * raise;
      TRGB t(rgb);
      t += p;
      parent->vertborder->putpix(0, top + y, cm(t));
    }
    BBox bb(right - 1 - x, top + x, right - x, bottom - 1 - x);
    bb &= redrawbbox;
    if (bb) {
      QPoint pp(bb.topleft());
      bb.shift(-right + 1 + x, 0);
      parent->vertborder->partput(bb, pp);
    }
  }
}
