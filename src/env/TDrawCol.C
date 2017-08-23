// TDrawCol.C

#define TPRIVATE

#include "TDrawCol.H"
#include "TSprite.H"
#include "setclip.H"

TDrawCol::TDrawCol(const TGFX &tgfx0, const TRGB &fg):
  tgfx(tgfx0) {
  XGCValues gcv;
  gcv.foreground = tgfx.rgb(fg);
  gcv.background = tgfx.grey(0);
  gc = XCreateGC(tgfx.disp(), tgfx.win(), GCForeground | GCBackground, &gcv);
}

TDrawCol::TDrawCol(const TGFX &tgfx0, TCol fg):
  tgfx(tgfx0) {
  XGCValues gcv;
  gcv.foreground = fg;
  gcv.background = tgfx.grey(0);
  gc = XCreateGC(tgfx.disp(), tgfx.win(), GCForeground | GCBackground, &gcv);
}

TDrawCol::~TDrawCol() {
  if (gc)
    XFreeGC(tgfx.disp(), gc);
}

void TDrawCol::line(int x0, int y0, int x1, int y1) const {
  XDrawLine(tgfx.disp(), tgfx.win(), gc, x0, y0, x1, y1);
}

void TDrawCol::rline(int x0, int y0, int dx, int dy) const {
  XDrawLine(tgfx.disp(), tgfx.win(), gc, x0, y0, x0 + dx, y0 + dy);
}

void TDrawCol::point(int x0, int y0) const {
  XDrawPoint(tgfx.disp(), tgfx.win(), gc, x0, y0);
}

void TDrawCol::rectangle(int x0, int y0, int w, int h, bool fill) const {
  if (fill)
    XFillRectangle(tgfx.disp(), tgfx.win(), gc, x0, y0, w, h);
  else
    XDrawRectangle(tgfx.disp(), tgfx.win(), gc, x0, y0, w - 1, h - 1);
}

void TDrawCol::ellipse(int x0, int y0, int w, int h, bool fill) const {
  if (fill)
    XFillArc(tgfx.disp(),
             tgfx.win(), gc, x0 - w, y0 - h, 2 * w, 2 * h, 0, 360 * 64);
  else
    XDrawArc(tgfx.disp(),
             tgfx.win(), gc, x0 - w, y0 - h, 2 * w, 2 * h, 0, 360 * 64);
}

void TDrawCol::setclip(BBox const &bb) {
  ::setclip(tgfx, gc, bb);
}