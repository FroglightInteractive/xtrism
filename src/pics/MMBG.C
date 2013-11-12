// MMBG.C

#include "MMBG.H"
#include "MMPict.H"
#include "../env/TImageFile.H"
#include "../env/TEnv.H"
#include "../bytemap/RGBImage.H"

MMBG::MMBG(TEnv const &env, TImageFile *timf):
  TImage(env, env.width(), env.height()) {
  if (timf && timf->readable())
    { timf->read(this); return; }
  // we'll have to build it from scratch...
  printf("Making main menu background (this may take a while)\n");
  MMPict mmp(env.width(), env.height());
  RGBDither rgbd(*this,*mmp.rgbmap(), (8-env.cmap().avecoldep())>?0);
  rgbd.dither();
  if (timf)
    timf->write(this);
  }
