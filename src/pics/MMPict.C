// MMPict.C

#include "MMPict.H"
#include "MidPtDisp.H"
#include "../bytemap/RGBMap.H"
#include <math.h>
#include "../basics/minmax.H"

MMPict::MMPict(int w0, int h0, MidPtDisp const *tb_cloud0,
               RGBMap *rgbmap0, bool doitnow):
  wid(w0), hei(h0) {
  if (rgbmap0)
    { rgbm=rgbmap0; rgbismine=false; }
  else
    { rgbm=new RGBMap(wid,hei); rgbismine=true; }
  if (tb_cloud0)
    { tb_cloud=tb_cloud0; cloudismine=false; }
  else
    { tb_cloud=new MidPtDisp(log(wid)/log(2));  cloudismine=true; }
  if (doitnow)
    drawit(0,hei);
  }

MMPict::~MMPict() {
  if (cloudismine)
    delete tb_cloud;
  if (rgbismine)
    delete rgbm;
  }


const float HAZE1=530;
const float HAZE0=-100;
const float HAZE1a=.1;
const float CLOUD1=.09;
const float CLOUD0=1500;
const float CLOUD1b=1;
// const float dGREY=0; 
const float GREY0=256;

const float HORISCR=.75; // Don't change w/o changing the haze calc's

const float H_0=3.5;
const float Z_HORI=20.;
const float BETA=1/(Z_HORI*Z_HORI);
const float DELTA=.51;
const float dXI=.05;
const float dZETA=.05;

const float dRED  =.0025;
const float dGREEN=.0023;
const float dBLUE =.000810;
const float RED0  =.000;
const float GREEN0=.000;
const float BLUE0 =.500;

const float H_g=.05;
const float BETA_g=-1/(Z_HORI*Z_HORI*.9);

const float HAZE1_g=520;
const float HAZE0_g=-100;
const float HAZE1a_g=.15;
const float CLOUD1_g=.05;
const float CLOUD2_g=30;
const float CLOUD2f_g=1.75;
const float CLOUD0_g=2200;
const float CLOUD1b_g=1;
const float GREY0_g=256;
const float dXI_g=.075;
const float dZETA_g=.1;

const float dREDg  =-.00045;
const float dGREENg=-.00028;
const float dBLUEg =-.00041;
const float hREDg  =.0018;
const float hGREENg=.0007;
const float hBLUEg =.0018;
const float RED0g  =.000;
const float GREEN0g=.600;
const float BLUE0g =.000;

const float ROUGHg = 1800;
const float ROUGHFREQg=.02;

/* Mathematics:
   y = (delta / (delta+zeta)) h(zeta,xi)
   x = (delta / (delta+zeta)) xi
   h(zeta,xi) = h_0 (1 - beta (xi^2 + zeta^2))
   beta = 1/zeta_hori^2
   delta = zeta_viewer vs zeta_screen
   h_0 = height of clouds
 * zeta() and xi() calculate the inverse of this transformation
 */

inline float zeta(float x,float y) { // z coordinate for clouds
  float x2=x*x;
  float b=y+2*BETA*H_0*x2;
  float a=DELTA*BETA*H_0+BETA*H_0*x2/DELTA;
  float c=DELTA*(y-H_0)+DELTA*BETA*H_0*x2;
  return (sqrt(b*b-4*a*c)-b)/(2*a);
  }

inline float zetag(float x, float y) { // z coord for ground
  float x2=x*x;
  float b=y+2*BETA_g*H_g*x2;
  float a=DELTA*BETA_g*H_g+BETA_g*H_g*x2/DELTA;
  float c=DELTA*(y-H_0)+DELTA*BETA_g*H_0*x2;
  return (sqrt(b*b-4*a*c)-b)/(2*a);
  }

inline float xi(float x, float zeta) { // x coordinate for clouds & ground
  return x*(DELTA+zeta)/DELTA;
  }

void MMPict::drawit(int top, int bottom) {
  int hori=int(HORISCR*hei);
  int th=tb_cloud->height();
  int tw=tb_cloud->width();
  
  int b2=min(hori,bottom);
  int t2=max(hori,top);
  
  for (int y=top; y<b2; y++)
    { // doing sky
      float eta=float(y)/hei;
      float a=HORISCR-eta;
      float xp=exp(-a/HAZE1a);
      float c_e_3=HAZE1*xp + HAZE0;
      float c_e_0=CLOUD1*(1-CLOUD1b*xp);
      float grey=GREY0; // dGREY*eta+GREY0;
      for (int x=0; x<wid; x++)
        { float zeta=::zeta(float(x)/wid-.5,HORISCR-eta);
          float fty=th*zeta*dZETA; int ty=int(floor(fty)); fty-=ty;
          int ty2=(ty+1)&(th-1); ty&=(th-1);
          float xi=::xi((float(x)/wid-.5),zeta);
          float ftx=tw*xi*dXI; int tx=int(floor(ftx)); ftx-=tx;
          int tx2=(tx+1)&(tw-1); tx&=(tw-1);
          float cloud=( ((*tb_cloud)(tx,ty)*(1-ftx)+
                         (*tb_cloud)(tx2,ty)*ftx)*(1-fty) +
                        ((*tb_cloud)(tx,ty2)*(1-ftx)+
                         (*tb_cloud)(tx2,ty2)*ftx)*fty ) +
            CLOUD0;
          cloud *= c_e_0; // damp cloudyness near horizon
          cloud=cloud>?0;
          cloud += c_e_3; // add haze
          cloud=cloud>?0;
/*          float a=cos(xi*5), b=cos(zeta*5);
            cloud+=300*exp(-a*a*100);
            cloud+=300*exp(-b*b*100); */ // for grid
          float red=cloud*dRED+RED0;
          float green=cloud*dGREEN+GREEN0;
          float blue=cloud*dBLUE+BLUE0;
          rgbm->rset(x,y,int(red*grey),int(green*grey),int(blue*grey));
        }
    }
  for (int y=t2; y<bottom; y++)
    { // doing grass
      float a=float(y)/hei - HORISCR;
      float xp=exp(-a/HAZE1a_g);
      float haze=(HAZE1_g*xp + HAZE0_g)>?0;
      float c_e_0=CLOUD1_g*(1-CLOUD1b_g*xp);
      float grey=GREY0_g;
      for (int x=0; x<wid; x++)
        { float zeta=::zetag(float(x)/wid-.5,a);
          float fty=th*zeta*dZETA_g; int ty=int(floor(fty)); fty-=ty;
          int ty2=(ty+1)&(th-1); ty&=(th-1);
          float xi=::xi((float(x)/wid-.5),zeta);
          float ftx=tw*xi*dXI_g; int tx=int(floor(ftx)); ftx-=tx;
          int tx2=(tx+1)&(tw-1); tx&=(tw-1);
          float cloud=( ((*tb_cloud)(tx,ty)*(1-ftx)+
                         (*tb_cloud)(tx2,ty)*ftx)*(1-fty) +
                        ((*tb_cloud)(tx,ty2)*(1-ftx)+
                         (*tb_cloud)(tx2,ty2)*ftx)*fty ) +
            CLOUD0_g;
          cloud += ROUGHg*cos(cloud*ROUGHFREQg);
          cloud *= c_e_0; // damp cloudyness near horizon
          cloud += CLOUD2_g*cos(xi*CLOUD2f_g);
          cloud=cloud>?0;
          float red=cloud*dREDg+RED0g+haze*hREDg;
          float green=cloud*dGREENg+GREEN0g+haze*hGREENg;
          float blue=cloud*dBLUEg+BLUE0g+haze*hBLUEg;
          rgbm->rset(x,y,int(red*grey),int(green*grey),int(blue*grey));
        }
    }
  }
