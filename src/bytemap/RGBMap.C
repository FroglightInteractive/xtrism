// RGBMap.C

#include <stdlib.h>
#include <errno.h>
#include "RGBMap.H"
#include "../basics/dbx.H"

RGBMap::RGBMap(unsigned int w, unsigned int h):
  wid(w), hei(h) {
  dat=new RGB[wid*hei];
  memset(dat, 0, sizeof(RGB)*wid*hei);
  }

RGBMap::RGBMap(unsigned int w, unsigned int h, FILE *src):
  wid(w), hei(h) {
  dat=new RGB[wid*hei];
  int res=fread(dat,wid*sizeof(RGB),hei,src);
  if (res!=1)
    { if (ferror(src))
        athrow(string("RGBMap: cannot read data") + strerror(errno));
      else
        athrow("RGBMap: unexpected EOF");
    }
  }

RGBMap::~RGBMap() {
  delete [] dat;
  }

RGBMap::RGBMap(RGBMap const &oth, unsigned int x, unsigned int y,
                 unsigned int w, unsigned int h): wid(w), hei(h) {
  dat=new RGB[wid*hei];
  for (unsigned int a=0; a<hei; a++)
    memcpy(line(a), oth.cline(y+a)+x, wid*sizeof(RGB)); 
  }

RGBMap::RGBMap(RGBMap const &oth, int rot): wid(oth.wid), hei(oth.hei) {
  if (hei!=wid)
    athrow("RGBMap: cannot rotate non-square map");
  dat=new RGB[wid*hei];
  switch (rot & 3)
    {
    case 0: memcpy(dat,oth.dat,sizeof(RGB)*wid*hei); break;
    case 1:
      for (unsigned int y=0; y<hei; y++)
        for (unsigned int x=0; x<wid; x++)
          c(x,y)=oth.cc(y,wid-1-x);
      break;
    case 2:
      for (unsigned int y=0; y<hei; y++)
        for (unsigned int x=0; x<wid; x++)
          c(x,y)=oth.cc(hei-1-y,wid-1-x);
      break;
    case 3:
      for (unsigned int y=0; y<hei; y++)
        for (unsigned int x=0; x<wid; x++)
          c(x,y)=oth.cc(wid-1-y,x);
      break;
    }
  }

void RGBMap::write(FILE *dst) const {
  if (fwrite(dat,wid*sizeof(RGB),hei,dst)!=hei)
    athrow(string("RGBMap: Cannot write data: ")+strerror(errno));
  };
