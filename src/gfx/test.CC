// env/test.C

#include <string>
#include <stdlib.h>
#include <stdio.h>

#include "../env/TEnv.H"
#include "TopBox.H"
#include "Button.H"
#include "../bricks/bsprites.H"
#include "ShowBrick.H"

class Bla: public Button {
  public:
    Bla::Bla(const Area &area, const TGFX &tgfx, const TRGB &bg,
             ShowBrick **t):
      Button(area,tgfx,bg), to(t) {}
    virtual void motion(const Point &p)
      { for (int i=0; i<34; i++) to[i]->rotate(); }
    ShowBrick **to;
  };    

int main(int argc, char **argv) {
  try
    {
      TEnv tenv(TReso(80,60,4,30),argc,argv);
      TMFont tmf(tenv,"-*-lucida-*-r-*-*-15-*-*-*-*-*-*-*");
      TFont tf(tmf,TRGB(255),TRGB(0));
      printf("making sbrickdata\n");
      SBrickData sbd("/root/xtrism/data/bricks");
      printf("making bricksprites\n");
      BrickSprites bspr(sbd,Filename("/root/xtrism/data"),20,tenv);
      printf("making boxes\n");
      TopBox topbox(tenv,BBox(0,0,960,720));
      ShowBrick *bt[34];
      bt[0]=new ShowBrick(Area(80,80),
                          tenv,TRGB(0,0,0),bspr,sbd,0,0);
      for (int i=1; i<34; i++)
        bt[i]=new ShowBrick(Area(80,80),
                            tenv,TRGB(0,0,0),bspr,sbd,i,0);

      Bla bla(Area(80,80),
              tenv,TRGB(64,64,64), bt);

      topbox.newchild(GBChildInfo(bt[0],
                                  GBPos(&topbox,40),
                                  GBPos(&topbox,40),
                                  GBPos(bt[1],40),
                                  GBPos(bt[6],40)));
      for (int i=1; i<6; i++)
        topbox.newchild(GBChildInfo(bt[i],
                                    GBPos(bt[i-1],40),
                                    GBPos(bt[0]),
                                    GBPos(i==5?&topbox:bt[i+1],40),
                                    GBPos()));
      for (int i=6; i<34; i+=6)
        topbox.newchild(GBChildInfo(bt[i],
                                    GBPos(bt[0]),
                                    GBPos(bt[i-6],40),
                                    GBPos(),
                                    GBPos(i+6>=34?&topbox:bt[i+6],40)));

      for (int i=6; i<34; i+=6)
        for (int j=1; j<6 && j+i<34; j++)
          topbox.newchild(GBChildInfo(bt[i+j],
                                      GBPos(bt[i+j-6]),
                                      GBPos(bt[i+j-1]),
                                      GBPos(), GBPos()));
      topbox.newchild(GBChildInfo(&bla,
                                  GBPos(bt[34-6]),
                                  GBPos(bt[34-1]),
                                  GBPos(), GBPos()));
      printf("placechildren\n");
      topbox.placechildren();
                                               
      printf("entering topbox\n");
      topbox.reenter();
      printf("loop!\n");
      tenv.loop();
    }
  catch(string &x)
    { fprintf(stderr,"env/test: %s\n",x.c_str());
      exit(1);
    }
  catch(char *x)
    { fprintf(stderr,"env/test: %s\n",x);
      exit(1);
    }
  return 0;
  }

