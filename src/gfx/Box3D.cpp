// Box3D.C

#include "Box3D.h"
#include "../pics/BoxMarbler.h"

void Box3D::redraw(BBox const &bb) {
  bm->draw(bbox(), bb, +1, 3);
}