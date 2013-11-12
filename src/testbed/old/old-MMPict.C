// MMPict.C

#include "MMPict.H"
#include "../pics/MidPtDisp.H"
#include "../bytemap/RGBMap.H"
#include <math.h>

MMPict::MMPict(int w0, int h0, MidPtDisp const &tb_cloud0,
               RGBMap *rgbmap0, bool doitnow):
  wid(w0), hei(h0), tb_cloud(tb_cloud0) {
  if (rgbmap0)
    { rgbm=rgbmap0; ismine=false; }
  else
    { rgbm=new RGBMap(wid,hei); ismine=true; }

  if (doitnow)
    drawit(0,hei);
  }

MMPict::~MMPict() {
  if (ismine)
    delete rgbm;
  }

const float HAZE2 = 5*144.;
const float HAZE3 = 5*54.;
const float GREY0 = 2*480./(1<<16);
const float dGREY = 3*1024./(1<<16);
const float BLUE0 = 8192.;
const float GREEN0 = 4096;
const float dGREEN = .08*16.;
const float dRED = .12*32.;

void MMPict::drawit(int top, int bottom) {
  int hori=3*hei/4;
  int th=tb_cloud.height();
  int tw=tb_cloud.width();

  int b2=hori<?bottom;
  
  for (int y=top; y<b2; y++)
    { float a=float(y)/hei;
      float fty=th*a*a*a;
      int ty=floor(fty); fty-=ty;
      int ty2=(ty+1)&(th-1);
      float invy=.2/(1-a);
      float grey=a*dGREY + GREY0;
      for (int x=0; x<wid; x++)
        { float ftx=(x-wid/2)*invy;
          int tx=floor(ftx); ftx-=tx; tx&=(tw-1);
          int tx2=(tx+1)&(tw-1);
          float cloud=(tb_cloud(tx,ty)*(1-ftx)+tb_cloud(tx2,ty)*ftx)*(1-fty)+
            (tb_cloud(tx,ty2)*(1-ftx)+tb_cloud(tx2,ty2)*ftx)*fty;
          float y2=a*a;
          float y3=y2*a;
          cloud *= 1. - y3*(64./27); // damp cloudyness near horizon
          cloud += HAZE2 * y2*(16./9);  // add haze
          cloud += HAZE3 * y3*(64./27); // add more haze
          float red=cloud*dRED;
          float green=cloud*dGREEN+GREEN0;
          float blue=BLUE0;
          rgbm->rset(x,y,int(red*grey),int(green*grey),int(blue*grey));
        }
    }
  }

#if 0













      
  
  for (int y=0; y<horizon; y++)
    { float a=float(y)/hei+0.001;
      tb_y[y]=int(TBLSIZE*a*a*a);
      tb_invy[y]=int(INVBASE/((hei-y)/(hei/5.)));
    }
  for (int y=hei-horizon-1; y>=0; y--)
    { float a=(hei-horizon-y)/(hei/2.)+.001;
      tb_g[y]=int(TBLSIZE*a*a*a);
      tb_invg[y]=int(INVBASE/((5.+y)/(hei/5.)));
    }
  }

MMPict::~MMPict() {
  delete [] tb_invy;
  delete [] tb_y;
  delete [] tb_g;
  delete [] tb_invg;
  }

void MMPict::getrgb(int x, int y, byte &r, byte &g, byte &b) {
  int xx, yy;
  float cloud;
  if (y<horizon)
    { // air part
      xx=x-wid/2;
      yy=tb_invy[y];
      xx*=yy; xx>>=LOGINVBASE; xx&=TBLSIZE-1;
      yy=tb_y[y];
      cloud=tb_cloud(xx,yy);
      int y2=y*y;
      int y3=y2*y;
      cloud=cloud*(hori3-y3)/hori3; // damp cloudyness near horizon
      cloud+=144*y2/hori2; // add haze
      cloud+=54*y3/hori3; // add more haze
      /* blue' = 1<<13;
         green' = (cloud + 256) << 4
         red' = cloud << 5
         c = y*4 + 480
         (red,green,blue) = c * (red',green',blue') // each 24 bits
       */
    }
  else
    { // ground part
    }

#endif

