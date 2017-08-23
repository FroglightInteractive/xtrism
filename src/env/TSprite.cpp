// TSprite.C

#define TPRIVATE

#include <X11/Xlib.h>
#include "TSprite.H"
#include <stdio.h>
#include "../basics/BBox.H"
#include "TEnv.H"
#include "TImage.H"
#include "setclip.H"

TSprite::TSprite(const TGFX &tgfx0, int w, int h):
  tgfx(tgfx0), wid(w), hei(h) {
  init();
}

TSprite::TSprite(const TEnv &tenv0, int w, int h):
  tgfx(tenv0), wid(w), hei(h) {
  init();
}

TSprite::TSprite(const TImage &timage):
  tgfx(timage.tenv()), wid(timage.width()), hei(timage.height()) {
  init();
  read(timage);
}

void TSprite::read(const TImage &timage) {
  XPutImage(tgfx.disp(), pixmap, gc, timage.ximage(), 0, 0, 0, 0, wid, hei);
}

void TSprite::read(const TImage &timage, BBox const &src, Point const &dst) {
  XPutImage(tgfx.disp(), pixmap, gc, timage.ximage(),
            src.left(), src.top(),
            dst.x(), dst.y(), src.width(), src.height());
}

void TSprite::init() {
  pixmap = XCreatePixmap(tgfx.disp(), tgfx.win(), wid, hei,
                         DefaultDepth(tgfx.disp(),
                                      DefaultScreen(tgfx.disp())));
  XGCValues gcv;
  gcv.tile = pixmap;
  gcv.fill_style = FillTiled;
  gc = XCreateGC(tgfx.disp(), tgfx.win(), GCTile | GCFillStyle, &gcv);
}

TSprite::TSprite(const TSprite &other):
  tgfx(other.tgfx), wid(other.wid), hei(other.hei) {
  pixmap = XCreatePixmap(tgfx.disp(), tgfx.win(), other.wid, other.hei,
                         DefaultDepth(tgfx.disp(),
                                      DefaultScreen(tgfx.disp())));
  XCopyGC(tgfx.disp(), other.gc, (1 << 23) - 1, gc);
  XCopyArea(tgfx.disp(), other.pixmap, pixmap,
            gc,
            0, 0, wid, hei, 0, 0);
}

TSprite::~TSprite() {
  if (pixmap)
    XFreePixmap(tgfx.disp(), pixmap);
  if (gc)
    XFreeGC(tgfx.disp(), gc);
}

void TSprite::tilerectangle(BBox const &to) const {
  XFillRectangle(tgfx.disp(), tgfx.win(), gc,
                 to.left(), to.top(), to.width(), to.height());
}

void TSprite::put(int x, int y) const {
  XCopyArea(tgfx.disp(), pixmap, tgfx.win(), gc, 0, 0, wid, hei, x, y);
}

void TSprite::partput(BBox const &from, Point const &to) const {
  XCopyArea(tgfx.disp(), pixmap, tgfx.win(), gc,
            from.left(), from.top(), from.width(), from.height(),
            to.x(), to.y());
}

void TSprite::setclip(BBox const &bb) {
  ::setclip(tgfx, gc, bb);
}