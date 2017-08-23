// bsprites.C

/* BrickSprites is a bucket containing TSprites corresponding to every cell
   in a brickset. Given an SBrickData, it can construct itself automatically,
   creating BCImageFiles and BCellFiles if necessary.
 */

#include "data.H"
#include "bsprites.H"
#include "brickcell.H"
#include "../basics/Throw.H"
#include "../basics/Filename.H"
#include "../basics/dbx.H"
#include "../env/TImage.H"
#include "../env/TSprite.H"
#include "../env/TImageFile.H"
#include "../bytemap/WideRGB.H"
#include "../bytemap/ByteImage.H"

#include <stdio.h>

//////////////////////////////// BrickSprites ////////////////////////////////
BrickSprites::BrickSprites(const SBrickData &sbd, const Filename &basename,
                           int size, const TEnv &env, int style):
  cells(0), myresp(0) {
  dbx(1, "BrickSprites::BrickSprites");

  // create base filename : datadir/bci-SIZE
  Filename base(basename);
  char buf[10];
  sprintf(buf, "%i", size);
  base.addpart(buf);

  /* create TSprites (in a strange way, because array constructors can't
     take arguments. */
  n = BD_MAXCELL * BD_MAXROT * sbd.number();
  cells = new TSprite *[n];
  for (int i = 0; i < n; i++)
    cells[i] = 0;
  myresp = new bool[n];

  // opening image file
  TImageFile timf(env, base);
  TImage tim(env, size, size);
  BrickCell *mother = 0, *rotd = 0, *cell = 0;
  WideRGB rgb0, rgb1;
  bool create = !timf.readable();

  // reading images (if nec. creating them)
  dbx(2, "BS: going to read images. create=%i", create);
  unsigned int nn = sbd.number();
  for (unsigned int bno = 0; bno < nn; bno++) {
    RBrickData const &rbd = sbd[bno];
    if (create) {
      mother = new BrickCell(size * rbd.height());
      switch (style) {
      case 0:
        break;

      case 1:
        mother->drawstripes();
        break;
      }
      rgb0 = TRGB(rbd.colour());
      rgb1 = rgb0;
      rgb1.r += 100;
      rgb1.g += 100;
      rgb1.b += 100;
      rgb0.r -= 100;
      rgb0.g -= 100;
      rgb0.b -= 100;
    }
    switch (rbd.rotstyle()) {
    case BrickData::ROT_4x4:
    case BrickData::ROT_4x4_TWO:
    case BrickData::ROT_3x3:
      for (unsigned int rot = 0; rot < 4; rot++) {
        if (create && rot)
          rotd = new BrickCell(*mother, rot);
        for (unsigned int cel = 0; cel < 4; cel++) {
          if (create) {
            cell = docreate(cel, rotd ? rotd : mother, size, rbd, rot);
            byteimage(tim, *cell, rgb0, rgb1);
            timf.write(&tim);
            delete cell;
          } else {
            timf.read(&tim);
          }
          (cells[cel + BD_MAXCELL * (rot + BD_MAXROT * bno)]
             = new TSprite(env, size, size))->read(tim);
          myresp[cel + BD_MAXCELL * (rot + BD_MAXROT * bno)] = true;
          dbx(-980804, "bs:cel=%i rot=%i bno=%i size=%i create=%i",
              cel, rot, bno, size, create);
        }
        if (rotd)
          delete rotd;
        rotd = 0;
      }
      break;

    default:
      athrow("BrickSprites: undefined rotation style");
    }
    if (mother)
      delete mother;
    mother = 0;
  }
  dbx(2, "BS: done");
}

BrickSprites::~BrickSprites() {
  if (cells) {
    for (int i = 0; i < n; i++)
      if (cells[i] && myresp && myresp[i])
        delete cells[i];
    delete[] cells;
  }
  if (myresp)
    delete[] myresp;
}

int BrickSprites::size() const {
  return cells[0]->width();
}

BrickCell *BrickSprites::docreate(int cel,
                                  const BrickCell *mother, int size,
                                  const RBrickData &rbd, int rot) {
  int x0 = rbd[0].x(cel), y0 = rbd[0].y(cel);
  int x, y;
  int maxxy = rbd.height() - 1;
  switch (rot & 3) {
  case 0:
    x = x0;
    y = y0;
    break;

  case 1:
    x = maxxy - y0;
    y = x0;
    break;

  case 2:
    x = maxxy - x0;
    y = maxxy - y0;
    break;

  case 3:
    x = y0;
    y = maxxy - x0;
    break;
  }
  BrickData const &bd = rbd[rot];
  BrickCell *bc = new BrickCell(*mother, size * x, size * y, size);
  x = bd.x(cel);
  y = bd.y(cel);
  BCSurround sur(bd.safecell(x + 1, y), bd.safecell(x + 1, y - 1),
                 bd.safecell(x, y - 1), bd.safecell(x - 1, y - 1),
                 bd.safecell(x - 1, y), bd.safecell(x - 1, y + 1),
                 bd.safecell(x, y + 1), bd.safecell(x + 1, y + 1));
  bc->drawborders(max(size / 10, 1), sur);
/*  for (int x=0; x<3*cel+3; x++)
    for (int y=0; y<4; y++)
    if (x%3)
    bc->c(x+3,y+3)=0; */// number the bricks for testing purps
  return bc;
}