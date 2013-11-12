// Box3D.H

#ifndef _Box3D_H
#define _Box3D_H

#include "GBox.H"
#include <stdio.h>
#include "../basics/Byte.H"

class Box3D: public GBox {
  public:
    Box3D(class GBParent *p, class Area const &area,
          class BoxMarbler *contents): GBox(p,area), bm(contents) { }
    virtual void redraw(class BBox const &bbox);
    virtual bool mouse(int b, bool io, const Point &p)
      { printf("Box3D: Click! %i/%i (%i,%i) (this=%p)\n",
               b,io,p.x(),p.y(),this); return true; }
  private:
    class BoxMarbler *bm;
  };

#endif
