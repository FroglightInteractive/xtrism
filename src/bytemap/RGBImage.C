// RGBImage.C

#include "../env/TImage.H"
#include "../env/TEnv.H"
#include "../basics/BBox.H"
#include "../basics/Throw.H"
#include "RGBMap.H"
#include "RGBImage.H"
#include "WideRGB.H"

void rgbimage(TImage &dst, RGBMap const &src) {
  rgbimage(dst,src,BBox(0,0,src.width(),src.height()));
  }

void rgbimage(TImage &dst, RGBMap const &src, BBox const &bb) {
  if (bb.top()<0 || bb.bottom()>int(src.height()) ||
      bb.left()<0 || bb.right()>int(src.width()))
    { dbx(1,"rgbimage: bad bbox: %i %i %i %i",
          bb.left(),bb.top(),bb.right(),bb.bottom());
      athrow("rgbimage: bad bbox");
    }
  TEnv const &tenv=dst.tenv();
  for (int y=bb.top(); y<bb.bottom(); y++)
    for (int x=bb.left(); x<bb.right(); x++)
      { RGB const &rgb=src.cc(x,y);
        dst.putpix(x,y,tenv.rgb(TRGB(rgb.r, rgb.g, rgb.b)));
      }
  }

RGBDither::RGBDither(TImage &dst0, RGBMap const &src0, unsigned int noise0):
  dst(dst0), src(src0), noise((1<<noise0)-1) {
  if (dst.width()<src.width() || dst.height()<src.height())
    athrow("RGBDither: Dest. must not be smaller than source");
  errs=new WideRGB[src.width()+2]; errs++;
  y_last=0;
  }

RGBDither::~RGBDither() {
  errs--;
  delete [] errs;
  }

void RGBDither::dither() {
  dither(0,src.height());
  }

void RGBDither::dither(int y_top, int y_bot) {
  fthrow(y_top==y_last,"RGBDither::dither: bad y_top: must be contiguous");
  fthrow(y_bot<=int(src.height()),"RGBDither::dither: bad y_bot");
  TCmap const &cmap=dst.tenv().cmap();
  int noisecor=(noise+1)/2;
  for (int y=y_top; y<y_bot; y++)
    { WideRGB xer;
      for (int x=0; x<int(src.width()); x++)
        { RGB const &rgb=src.cc(x,y);
          unsigned int a=random();
          WideRGB tryrgb(rgb.r + ((xer.r + errs[x].r)*5 +
                                  (errs[x-1].r+errs[x+1].r)*3)/20 +
                         (a & noise)-noisecor,
                         rgb.g + ((xer.g + errs[x].g)*5 +
                                  (errs[x-1].g+errs[x+1].g)*3)/20 +
                         ((a>>8)&noise)-noisecor,
                         rgb.b + ((xer.b + errs[x].b)*5 +
                                  (errs[x-1].b+errs[x+1].b)*3)/20 +
                         ((a>>16)&noise)-noisecor);
          TCol c=cmap(TRGB(tryrgb.r,tryrgb.g,tryrgb.b));
          TRGB resrgb(cmap(c));
          xer.r=tryrgb.r-resrgb.r;
          xer.g=tryrgb.g-resrgb.g;
          xer.b=tryrgb.b-resrgb.b;
          errs[x]=xer;
          dst.putpix(x,y,c);
        }
    }
  y_last=y_bot;
  }
