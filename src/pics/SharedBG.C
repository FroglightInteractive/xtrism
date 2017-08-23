// SharedBG.C

#include "SharedBG.H"
#include "../basics/Filename.H"
#include "../basics/Throw.H"
#include "../env/TEnv.H"
#include "../env/TImage.H"
#include "../env/TImageFile.H"
#include "../bytemap/ByteMap.H"

SharedBG::SharedBG(): img(0), bm(0), first(0) {
}

SharedBG::~SharedBG() {
  if (img)
    delete img;
  if (bm)
    delete bm;
}

void SharedBG::draw(TEnv const &env, Filename const &imgf, BBox const &mybbox,
                    bool usebytemap) {
  bb = mybbox;
  img = new TImage(env, bb.width(), bb.height());
  TImageFile timf(env, imgf);
  if (timf.readable())
    timf.read(img);
  else {
    if (usebytemap)
      fthrow((bm = makebm(bb)),
             "SharedBG: no drawing routine for ByteMap");
    else
      fthrow(draw(bb.topleft(), img),
             "SharedBG: no drawing routine for TImage");
    for (BGSharer *bgs = first; bgs != 0; bgs = bgs->next())
      if (usebytemap)
        fthrow(bgs->draw(bb.topleft(), bm),
               "SharedBG: BGSharer has no drawing routine for ByteMap");
      else
        fthrow(bgs->draw(bb.topleft(), img),
               "SharedBG: BGSharer has no drawing routine for TImage");
    if (usebytemap) {
      fthrow(convert(img, bm),
             "SharedBG: no convert routine");
      delete bm;
      bm = 0;
    }
    timf.write(img);
  }
}

void SharedBG::redraw(BBox const &bbox) const {
  fthrow(img, "SharedBG: No image");
  img->partput(BBox(bbox).shift(-bb.left(), -bb.top()), bbox.topleft());
}

void SharedBG::reg(BGSharer *child) {
  if (first)
    first->reg(child);
  else
    first = child;
}

BGSharer::BGSharer(SharedBG &par0): nxt(0), par(par0) {
  par0.reg(this);
}

BGSharer::~BGSharer() {
}

void BGSharer::reg(BGSharer *sib) {
  if (nxt)
    nxt->reg(sib);
  else
    nxt = sib;
}