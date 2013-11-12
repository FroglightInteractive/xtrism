// TCmap.C

#define TPRIVATE

#include "../basics/Throw.H"
#include "TCmap.H"

#include <stdio.h>

void TRGB::fill(const char *str) {
  { int rr,gg,bb;
    if (sscanf(str,"%i %i %i",&rr,&gg,&bb)==3)
      { r=rr; g=gg; b=bb; limit(); return; }
  }
  { double rr,gg,bb;
    if (sscanf(str,"%lg,%lg,%lg",&rr,&gg,&bb)==3)
      { r=int(255*rr); g=int(255*gg); b=int(255*bb); limit(); return; }
  }
  athrow(string("TRGB: Bad colour description: `")+str+"'");
  }

TCmap::~TCmap() {
  if (isprivate)
    XFreeColormap(disp,cmap);
  if (table)
    delete [] table;
  if (palette)
    delete [] palette;
  }

/* ------------------------------ staticinit ------------------------------ */
/* Initialises the (r/g/b)(right/left/mask/add) members */
/* Device independent */
void TCmap::staticinit(int rpos, int rbits, int gpos, int gbits,
                       int bpos, int bbits, int leftbits) {
  _avecoldep=(rbits+gbits+bbits+leftbits+1)/3;
  rright=8-rbits; rleft=rpos; rmask=(255>>rright)<<rpos; radd=127>>rbits;
  gright=8-gbits; gleft=gpos; gmask=(255>>gright)<<gpos; gadd=127>>gbits;
  bright=8-bbits; bleft=bpos; bmask=(255>>bright)<<bpos; badd=127>>bbits;
  greybits=leftbits;
  }

/* ------------------------------ dynamicinit (rgb) ----------------------- */
/* Fills a Colormap with rgb values:
   - bits 0:(leftbits-1): grey addition
   - next rbits bits = red value
   - next gbits bits = green value
   - next bbits bits = blue value
 * If leftbits or tabulate, produces lookup table to read colours.
 * calls staticinit().
 */
void TCmap::dynamicinit(int rbits, int gbits, int bbits, int leftbits,
                        bool tabulate) {
  XColor c;
  _avecoldep=(rbits+gbits+bbits+leftbits+1)/3;
  c.flags=DoRed|DoGreen|DoBlue;
  int nr=1<<rbits, ng=1<<gbits, nb=1<<bbits;
  int ngrey=1<<leftbits;
  int rmul=65535/(nr*ngrey-1),
    gmul=65535/(ng*ngrey-1),
    bmul=65535/(nb*ngrey-1);
  if (leftbits)
    tabulate=true;
  if (tabulate)
    { table=new TCol[16*16*16];
      palette=new TRGB[nr*ng*nb*ngrey];
    }
  for (int grey=0; grey<ngrey; grey++)
    for (int r=0; r<nr; r++)
      { c.red=(grey+r*ngrey)*rmul;
        for (int g=0; g<ng; g++)
          { c.green=(grey+g*ngrey)*gmul;
            for (int b=0; b<nb; b++)
              { c.blue=(grey+b*ngrey)*bmul;
                c.pixel=grey+(r<<leftbits)+(g<<(leftbits+rbits))
                  +(b<<(leftbits+rbits+gbits));
                XStoreColor(disp,cmap,&c);
                if (palette)
                  palette[c.pixel]=TRGB(c.red/256,c.green/256,c.blue/256);
              }
          }
      }
  if (tabulate)
    { for (int b=0; b<16; b++)
        for (int g=0; g<16; g++)
          for (int r=0; r<16; r++)
            table[r+16*g+16*16*b]=nearest(r,g,b,ngrey,nr,ng,nb);
    }
  staticinit(0,rbits, rbits,gbits, rbits+gbits,bbits,leftbits);
  }

TCol TCmap::nearest(int r,int g,int b, int ngrey, int nr, int ng, int nb) {
  int red=(r*nr)/16;
  int green=(g*ng)/16;
  int blue=(b*nb)/16;
  r-=(red*16)/nr;
  g-=(green*16)/ng;
  b-=(blue*16)/nb;
  int grey=2*r*256*nr/16 + 3*g*256*ng/16 + b*256*nb/16;
  grey=(grey*ngrey)/(256*6);
  return grey+ngrey*(red+nr*(green+ng*blue));
  }
  

/* ------------------------------ dynamicinit (grey) ---------------------- */
/* Fills a Colormap with grey values:
   - bits 0:(greybits-1) = red value = green value = blue value
 * Calls staticinit().
 */
void TCmap::dynamicinit(int greybits) {
  XColor c;
  _avecoldep=greybits; // bit of a fake...
  c.flags=DoRed|DoGreen|DoBlue;
  tthrow(greybits<2,"TCmap: dynamicinit(greybits<2) - sorry cannot do it");
  int n,nbits;
  if (greybits==2)
    { n=4; nbits=1; }
  else
    { nbits=greybits-2; n=3*((1<<nbits)-1)+1; }
  int mul=65535/(n-1);
  for (int g=0; g<n; g++)
    { c.red=c.green=c.blue=g*mul;
      c.pixel=g;
      XStoreColor(disp,cmap,&c);
    }
  staticinit(0,greybits-1, 0,greybits-1, 0,greybits-1);
  }


/* ------------------------------ tcmap_log2 ------------------------------ */
/* Returns the 2log of its input, rounded down, eg. log2(7)==2.
 * NB Feature: log2(0)==0 !!!
 */
static int tcmap_log2(unsigned int a) {
  int b=0;
  while (a>1)
    { b++; a>>=1; }
  return b;
  }

/* ------------------------------ tcmap_bit0 ------------------------------ */
/* Returns the bit number of the first non-zero bit in its input or -1 */
static int tcmap_bit0(unsigned int a) {
  if (a==0)
    return -1;
  int b=0, c=1;
  do
    { if (a & c)
        return b;
      b++; c+=c;
    } while(1);
  }
    

/* -------------------------------- visual -------------------------------- */
void TCmap::findvisual() {
  int screen=DefaultScreen(disp);
  int depth=DefaultDepth(disp,screen);
  XVisualInfo xvi;
#ifdef TESTING
  int classes[]={ PseudoColor }; // TEST!
#else
  int classes[]={ TrueColor, PseudoColor, DirectColor, GrayScale,
                  StaticColor, StaticGray };
  if (depth<=8)
    { classes[0]=PseudoColor; classes[1]=TrueColor; }
#endif
  int nclas=sizeof(classes)/sizeof(int);
  for (int n=0; n<nclas; n++)
    { if (XMatchVisualInfo(disp,screen,depth,classes[n],&xvi))
        { vis=xvi.visual;
          return;
        }
    }
  athrow("TCmap: Cannot obtain acceptable visual - sorry");
  }

/* ------------------------------ constructor ----------------------------- */
TCmap::TCmap(Display *d):
  disp(d), cmap(0), isprivate(0),
    palette(0), table(0) {
  findvisual();
  switch (vis->c_class)
    {
    case PseudoColor: {
      cmap=XCreateColormap(disp,DefaultRootWindow(disp),vis,AllocAll);
      isprivate=true;
      int bits=tcmap_log2(vis->map_entries);
      if (bits<2)
        athrow("TCmap: cannot play Trism with less than 4 colours");
      else if (bits<3)
        dynamicinit(bits);
      else if (bits<=8)
        { int rgbb=bits/3;
          dynamicinit(rgbb,rgbb,rgbb,bits-3*rgbb,true);
        }
      else
        { int bbits=bits/3; bits-=bbits;
          int rbits=bits/2; bits-=rbits;
          int gbits=bits;
          dynamicinit(rbits,gbits,bbits);
        }
      } return;
    case TrueColor: {
      if (vis==DefaultVisual(disp,DefaultScreen(disp)))
        { cmap=DefaultColormap(disp,DefaultScreen(disp)); isprivate=false; }
      else
        { cmap=XCreateColormap(disp,DefaultRootWindow(disp),vis,AllocNone);
          isprivate=true;
        }
      int rpos=tcmap_bit0(vis->red_mask);
      int rbits=tcmap_bit0((vis->red_mask>>rpos)+1);
      int gpos=tcmap_bit0(vis->green_mask);
      int gbits=tcmap_bit0((vis->green_mask>>gpos)+1);
      int bpos=tcmap_bit0(vis->blue_mask);
      int bbits=tcmap_bit0((vis->blue_mask>>bpos)+1);
      staticinit(rpos,rbits, gpos,gbits, bpos,bbits);
      } return;
    case DirectColor:
      athrow("TCmap: DirectColor visual not yet implemented");
      break;
    case GrayScale: {
      cmap=XCreateColormap(disp,DefaultRootWindow(disp),vis,AllocAll);
      isprivate=true;
      dynamicinit(tcmap_log2(vis->map_entries));
      } return;
    case StaticColor:
      athrow(
        "TCmap: cannot play Trism in StaticColor visual - sorry");
      break;
    case StaticGray:
      athrow("TCmap: cannot play Trism in StaticGray visual - sorry");
      break;
    }
  athrow("TCmap: cannot play Trism in any of your visuals - sorry");
  }


string TCmap::id() const {
  char buf[20];
  if (rleft==gleft && rleft==bleft)
    sprintf(buf,"xgrey%i",8-rright);
  else
    { int a=(gleft<rleft) + 2*(bleft<gleft) + 4*(bleft<rleft);
      sprintf(buf,"x%i%i%i%c",8-rright,8-gright,8-bright,a+'a');
      if (table)
        sprintf(buf+strlen(buf),"%i",greybits);
    }
  return string(buf);
  }
