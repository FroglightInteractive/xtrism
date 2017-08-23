// Button.C

#include "Button.h"

#include "../env/TEnv.h"
#include "../env/TImage.h"
#include "../bytemap/ByteMap.h"
#include "../bytemap/ByteImage.h"

Button::Button(GBParent *p, Area const &area, const TRGB &bg):
  GBox(p, area) {
  tdc = new TDrawCol(p->tgfx(), bg);
  type = COLOUR;
}

Button::Button(GBParent *p, Area const &area):
  GBox(p, area) {
  type = TRANSP;
}

Button::Button(GBParent *p, class TImage const &image):
  GBox(p, Area(image.width(), image.height())) {
  img = &image;
  type = IMAGE;
}

Button::~Button() {
  switch (type) {
  case COLOUR:
    delete tdc;
    break;

  case IMAGE:
  case TRANSP:
    break;
  }
}

void Button::redraw(const BBox &bbox) {
  switch (type) {
  case TRANSP:
    break;

  case COLOUR:
    tdc->rectangle(left(), top(), width(), height());
    break;

  case IMAGE:
    img->partput(BBox(bbox).shift(-left(), -top()), bbox.topleft());
    break;
  }
}