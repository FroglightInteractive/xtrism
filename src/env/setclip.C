// setclip.C

#define TPRIVATE

#include <X11/Xlib.h>
#include "../basics/BBox.H"
#include "TGFX.H"
#include "setclip.H"

void setclip(TGFX const &tgfx, GC &gc, BBox const &bb) {
  XRectangle xr;
  xr.x=max(bb.left(),0); xr.y=max(bb.top(),0);
  xr.width=min(bb.width(),10000); xr.height=min(bb.height(),10000);
  // Yes! this is yucky. But it works: ever seen a screen > 10000 pix?
  // And if you have, bad luck. So there.
  XSetClipRectangles(tgfx.disp(),gc,0,0,&xr,1,YXBanded);
  }
