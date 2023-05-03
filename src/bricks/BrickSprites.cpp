// bsprites.C

/* BrickSprites is a bucket containing QPixmaps corresponding to every cell
   in a brickset. Given an SBrickData, it can construct itself automatically,
   creating BCImageFiles and BCellFiles if necessary.
 */

#include "BrickData.h"
#include "BrickSprites.h"
#include "BrickCell.h"

#include "RGBImage.h"

#include <stdio.h>

//////////////////////////////// BrickSprites ////////////////////////////////
BrickSprites::BrickSprites(const SBrickData &sbd, QString cachedir,
                           int size, int style) {
  // reading images (if nec. creating them)
  unsigned int nn = sbd.size();
  for (unsigned int bno = 0; bno < nn; bno++) {
    RBrickData const &rbd = sbd[bno];
    WideRGB rgb0 = rbd.color();
    WideRGB rgb1 = rbd.color();
    qDebug() << "Bricksprites" << bno << rbd.color() << rgb0.r << rgb0.g << rgb0.b;
    rgb1.r += 100;
    rgb1.g += 100;
    rgb1.b += 100;
    rgb0.r -= 100;
    rgb0.g -= 100;
    rgb0.b -= 100;
    BrickCell base(0, 0);
    for (unsigned int rot = 0; rot < BD_MAXROTS; rot++) {
      BrickCell rotd(0, 0);
      for (unsigned int cel = 0; cel < BD_MAXCELLS; cel++) {
        QString fn = QString("%1/brick-%2-%3-%4_%5-%6.png")
          .arg(cachedir)
          .arg(bno).arg(rot).arg(cel)
          .arg(size).arg(style);
        if (QFileInfo(fn).exists()) {
          RGBImage *img = new RGBImage(RGBImage::fromFile(fn));
          images << img;
          cells << QPixmap::fromImage(img->toQImage());
        } else {
          if (!base.width()) {
            base = BrickCell(size * rbd.height());
            if (style==1)
              base.drawstripes();
          }
          if (rot && !rotd.width())
            rotd = BrickCell(base.rotated(rot));
          BrickCell cell = docreate(cel, rot?rotd:base, size, rbd, rot);
          RGBImage *img = new RGBImage(RGBImage::colorized(cell, rgb0, rgb1));
          img->save(fn);
          images << img;
          cells << QPixmap::fromImage(img->toQImage());
        }
      }
    }
  }
}

BrickSprites::~BrickSprites() {
}

int BrickSprites::size() const {
  return cells[0].width();
}

BrickCell BrickSprites::docreate(int cel,
                                 BrickCell const &mother, int size,
                                 RBrickData const &rbd, int rot) {
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
  BrickCell bc(mother.cropped(size * x, size * y, size, size));
  x = bd.x(cel);
  y = bd.y(cel);
  BCSurround sur(bd.safecell(x + 1, y), bd.safecell(x + 1, y - 1),
                 bd.safecell(x, y - 1), bd.safecell(x - 1, y - 1),
                 bd.safecell(x - 1, y), bd.safecell(x - 1, y + 1),
                 bd.safecell(x, y + 1), bd.safecell(x + 1, y + 1));
  bc.drawborders(std::max(size / 10, 1), sur);
  return bc;
}
