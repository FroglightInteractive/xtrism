// Box3D.C

#include "Box3D.H"
#include "../pics/BoxMarbler.H"

void Box3D::redraw(BBox const &bb) {
  bm->draw(bbox(), bb, +1, 3);
}