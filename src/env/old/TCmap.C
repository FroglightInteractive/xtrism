// TCmap.C


#include "../Throw.H"
#include "TEnvP.H"

//////////////////////////////////////////////////////////////////////////////

/* ----------------------------- tcmap_rescale ---------------------------- */
/* given an integer x in the range 0..omax, returns an integer in the
   range 0..nmax, properly scaled. */
static int tcmap_rescale(int x, int omax, int nmax) { // impl.indep
  return int(((x+.0)/(omax+.000001)) * (nmax+.999999));
  }

/////////////////////////////////// tRGBmap //////////////////////////////////

/* --------------------------------- rgb ---------------------------------- */
/* Given a TRGB, returns the nearest available TCol. This function is not
   particularly fast, so better cache the results... */
TCol tRGBmap::rgb(const TRGB &rgb) const {
  return TCol(base +
              rmul*tcmap_rescale(rgb.r,255,rmax) +
              gmul*tcmap_rescale(rgb.g,255,gmax) +
              bmul*tcmap_rescale(rgb.b,255,bmax));
  }

/* -------------------------------- init ---------------------------------- */
void tRGBmap::init(int nr, int ng, int nb, int bbase,
                   Colormap cm, Display *d) {
  XColor c;
  c.flags=DoRed|DoGreen|DoBlue;
  for (int r=0; r<nr; r++)
    for (int g=0; g<ng; g++)
      for (int b=0; b<nb; b++)
        { c.red=tcmap_rescale(r,nr-1,65535);
          c.green=tcmap_rescale(g,ng-1,65535);
          c.blue=tcmap_rescale(b,nb-1,65535);
          c.pixel=bbase+r+nr*(g+ng*b);
          XStoreColor(d,cm,&c);
        }
  base=bbase;
  rmax=nr-1; rmul=1;
  gmax=ng-1; gmul=nr;
  bmax=nb-1; bmul=nr*ng;
  }

/////////////////////////////// tGreymap /////////////////////////////////////
TCol tGreymap::grey(int grey) const {
  return TCol(base+mul*tcmap_rescale(grey,255,max));
  }

void tGreymap::init(int n, int bbase,
                    Colormap cm,Display *d) {
  XColor c;
  c.flags=DoRed|DoGreen|DoBlue;
  for (int g=0; g<n; g++)
    { c.red=c.green=c.blue=tcmap_rescale(g,n-1,65535);
      c.pixel=bbase+g;
      XStoreColor(d,cm,&c);
    }
  base=bbase; max=n-1; mul=1;
  }
    

//////////////////////////////// TCmap ///////////////////////////////////////

/* --------------------------------- init --------------------------------- */
Visual *TCmap::init(Display *d) {
  int screen=DefaultScreen(d);
  int depth=DefaultDepth(d,screen);
  XVisualInfo xvi;
  Visual *v;
  int classes[5], nclas;
  if (depth>8)
    { classes[0]=TrueColor; classes[1]=PseudoColor; nclas=2; }
  else
    { classes[0]=PseudoColor; classes[1]=TrueColor; nclas=2; }
  classes[nclas++]=GrayScale;
  
  for (int n=0; n<nclas; n++)
    { if (XMatchVisualInfo(d,screen,depth,classes[n],&xvi))
        { v=xvi.visual;
          switch (xvi.c_class)
            {
            case PseudoColor: {
              cmap=XCreateColormap(d,DefaultRootWindow(d),v,AllocAll);
              int gn=xvi.colormap_size;
              int rgbn;
              for (rgbn=1; rgbn*rgbn*rgbn<=gn; rgbn++)
                ;
              rgbn--;
              if (rgbn>=3) // color possible
                { int left=gn-rgbn*rgbn*rgbn, base=0;
                  rgbmap.init(rgbn,rgbn,rgbn, left, cmap,d);
                  if (left>16)
                    { base=(left-16)<?7;
                      greymap.init(base,0,cmap,d);
                      left-=base; }
                  if (left>0)
                    greymap.init(left,base,cmap,d);
                  if (left<rgbn)
                    greymap.init(left, rgbn, 1+rgbn+rgbn*rgbn);
                  return v;
                }
              else if (gn>=4) // greyscale possible
                { greymap.init(gn,0,cmap,d);
                  rgbmap.init(0, (gn+2)/3,1, (gn+2)/3,1, (gn+2)/3,1);
                  return v;
                }
              else // black 'n white -> can't do it
                athrow("TCmap: cannot play Trism in monochrome");
              } break;
            case TrueColor:
              athrow("TCmap: TrueColor visual not yet implemented");
              break;
            case GrayScale:
              athrow("TCmap: GrayScale visual not yet implemented");
              break;
            }
        }
    }
  athrow("TCmap: Cannot deal with any of your visuals");
  return 0; // never executed
  }
