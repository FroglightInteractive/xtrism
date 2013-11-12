// MidPtDisp.C

#include "MidPtDisp.H"
#include <math.h>
#include <stdlib.h>
#include "../basics/Throw.H"
#include "../basics/dbx.H"

/* ------------------------------- normalrnd ------------------------------ */
// return value in [-446,446], more or less normally distr'd
inline int normalrnd() {
  unsigned int x=random();
  return (x&255) + ((x>>8)&255)
    + ((x>>16)&255) + ((x>>24)&127) - 446;
  }

static float fudge[]={1,2,4,9,21,48,64,100,170,200,300,400,300,200,100, 0};
static int nfu=sizeof(fudge)/sizeof(float);
inline float getfudge(int s) { if (s<0) throw "FOUT!"; return sqrt(fudge[s<nfu?s:nfu-1]); }

MidPtDisp::MidPtDisp(unsigned int pow, unsigned int seed) {
  srandom(seed);
  tthrow(pow>12,"MidPtDisp: won't do > 4096x4096");
  tthrow(pow<4,"MidPtDisp: won't do < 16x16");
  logsize=pow;
  size=1<<pow; sm=size-1;
  data=new float[size*size];
  for (int d=0; d<size*size; d++)
    data[d]=1e9;
  int st=2*logsize;
  float factor=getfudge(st--);
  
  C(0,0)=normalrnd()*factor;

  for (unsigned int step=size; step>1; step/=2)
    { int top,bot,y;
      int left,right,x;

      /* X X
          *
         X X
       */
      factor=getfudge(st--);
      for (top=0, bot=step, y=step/2;
           y<size;
           top+=step, bot+=step, y+=step)
        for (left=0, right=step, x=step/2;
             x<size;
             left+=step, right+=step, x+=step)
          { C(x,y)=(SC(left,top)+SC(left,bot)+SC(right,top)+SC(right,bot))/4+
              normalrnd()*factor;
//            if ((random()&63)==0)
//              dbx(2,"C(%i,%i)=%f",x,y,C(x,y));
          }

      /*  X
         X*X
          X
       */
      factor=getfudge(st--);
      for (top=-step/2, bot=step/2, y=0;
           y<size;
           top+=step, bot+=step, y+=step)
        for (left=0, right=step, x=step/2;
             x<size;
             left+=step, right+=step, x+=step)
          { C(x,y)=(SC(x,top)+SC(x,bot)+SC(left,y)+SC(right,y))/4+
              normalrnd()*factor;
//            if ((random()&63)==0)
//              dbx(2,"C(%i,%i)=%f",x,y,C(x,y));
          }
      for (top=0, bot=step, y=step/2;
           y<size;
           top+=step, bot+=step, y+=step)
        for (left=-step/2, right=step/2, x=0;
             x<size;
             left+=step, right+=step, x+=step)
          { C(x,y)=(SC(x,top)+SC(x,bot)+SC(left,y)+SC(right,y))/4+
              normalrnd()*factor;
//            if ((random()&63)==0)
//              dbx(2,"C(%i,%i)=%f",x,y,C(x,y));
          }
    }
  }
