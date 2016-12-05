// TestBed.H

#ifndef _TestBed_H
#define _TestBed_H

#include "../env/TEnv.H"
#include "../env/TImage.H"
#include "../env/TSprite.H"
#include "../bytemap/RGBMap.H"

class TestBed {
  public:
    TestBed(TEnv const &env);
  public:
    TEnv const &tenv;
    RGBMap rgbmap;
    TImage timage;
    TSprite tsprite;
  };
#endif
