// TEnv.C - Trisms portable environment: XWindows version

#define TPRIVATE
#define NICE

#ifdef NICE
#include <unistd.h>

const int USLEEP_ADVERTISED_COMPENSATION=10;
const int USLEEP_ACTUAL_COMPENSATION=20; // time lost in usleep(1) [ms]

#endif

bool tbusy;

#include <stdio.h>
#include "../basics/Throw.H"
#include "TEnv.H"
#include "TReso.H"
#include "TTime.H"
#include "../basics/Infty.H"
#include "../sound/SPlayer.H"

#include <X11/keysym.h>

TEnv::TEnv(const TReso &reso, int argc, char **argv, char *addit):
  TGFX(TGFX_MAGIC), confined(0), containspointer(0),
  repclaimed(0), wishautorep(0),
  lastkey(0),
  _bpp(0), _scanlpad(0) {

  // going to open the Display
  dbx(1,"TEnv::TEnv going to open display; addit=%p",addit);
  fthrow((display=XOpenDisplay(addit)),"TEnv: Cannot open display");
  dbx(2,".. display opened");
  screen=DefaultScreenOfDisplay(display);

  // going to determine optimal size for window
  int swidth=WidthOfScreen(screen);
  int sheight=HeightOfScreen(screen);
  actfactor=reso.best(swidth,sheight);

  if (reso.fullscreen()) {
    wid = swidth;
    hei = sheight;
  } else {
    wid=reso.actx(actfactor);
    hei=reso.acty(actfactor);
  }

  // going to create a suitable Colormap
  tcmap=new TCmap(display);
  visual=tcmap->visual();
  
  // going to open a window
  XSetWindowAttributes attr;
    attr.background_pixel=rgb(TRGB(0,0,0));
    attr.event_mask=(KeyPressMask|KeyReleaseMask|
                     ButtonPressMask|ButtonReleaseMask|
                     EnterWindowMask|LeaveWindowMask|
                     PointerMotionMask|ExposureMask);
    attr.override_redirect=False; // [True for full screen shell?]
    attr.colormap=tcmap->colormap();
    attr.cursor=None; // [something nice in later vsn?]
  drwb=window
    =XCreateWindow(display,DefaultRootWindow(display),  // display,parent
                   (swidth-wid)/2,(sheight-hei)/2,      // x,y
                   wid,hei,                             // w,h
                   0,DefaultDepthOfScreen(screen),      // border,depth
                   InputOutput,                         // class
                   visual,                              // visual
                   (CWBackPixel|CWEventMask|CWOverrideRedirect|
                    CWCursor|CWColormap)                // valuemask
                   ,&attr                               // attributes
                   );
  XSizeHints xsh;
    xsh.flags=PAllHints;
    xsh.x=(swidth-wid)/2; xsh.y=(sheight-hei)/2;
    xsh.min_width=xsh.max_width=xsh.width=wid;
    xsh.min_height=xsh.max_height=xsh.height=hei;
    xsh.width_inc=xsh.height_inc=1;
    xsh.min_aspect.x=xsh.max_aspect.x=wid;
    xsh.min_aspect.y=xsh.max_aspect.y=hei;
  XSetStandardProperties(display,window,
                         "XTrism","XTrism",
                         None,argv,argc,&xsh);

  /*
  XWMHints xwh;
    xwh.flags=AllHints;
    xwh.input=True; xwh.initial_state=NormalState;
    xwh.icon_pixmap=None; xwh.icon_window=None;
    xwh.icon_x=xwh.icon_y=0;
    xwh.icon_mask=None; xwh.window_group=None;
  XSetWMHints(display,window,&xwh);
  */

  XClassHint xch;
    xch.res_name="xtrism"; xch.res_class="XTrism";
  XSetClassHint(display,window,&xch);

  wmdelw=XInternAtom(display,"WM_DELETE_WINDOW",False);
  wmprot=XInternAtom(display,"WM_PROTOCOLS",False);
  fthrow(XSetWMProtocols(display,window,&wmdelw,1),
         "TEnv: Cannot set WM_DELETE_WINDOW property");
  XMapRaised(display,window);
  XMoveWindow(display,window,(swidth-wid)/2,(sheight-hei)/2);
  getpixmapformat();
  }

/* -------------------------------- loop ---------------------------------- */
/* Main loop */
void TEnv::loop(int nice) {
  dbx(1,"TEnv::loop()");
  dbx(3,"sync = %p",&sync);
  dbx(3,"poll = %p",&poll);
  quitting=false;
  while (!quitting)
    { if (spl)
        spl->poll();
      TTime tt(TTime::CURRENT);
      int nextpoll=poll?poll.top().tpoll(tt):INFTY;
      cbl.callpopall();
      bool moresync=sync?sync.top().tsync(tt):false;
      cbl.callpopall();
      if (XPending(display))
        { XEvent xev;
          XNextEvent(display,&xev);
          if (xev.xany.window==window)
            { switch (xev.type)
                {
                case KeyPress: case KeyRelease:
                  lastkey=&xev.xkey;
                  if (keyboard)
                    keyboard.top().tkey(xev.xkey.keycode,xev.type==KeyPress);
                  lastkey=0;
                  break;
                case ButtonPress: case ButtonRelease:
                  if (mml)
                    { mml.top().tmbc(button(xev.xbutton.button),
                                     xev.type==ButtonPress,
                                     xev.xbutton.x,xev.xbutton.y);
                      if (xev.type==ButtonRelease &&
                          ( xev.xbutton.state & (Button1Mask | Button2Mask |
                                                 Button3Mask | Button4Mask |
                                                 Button5Mask)
                            &~ buttonmask(xev.xbutton.button)) == 0)
                        mml.top().tmbc(0,0,xev.xbutton.x,xev.xbutton.y);
                      // As a special case, call tmbc(0,0,pt) when all 
                      // buttons are released.
                    }
                  break;
                case EnterNotify:
                  claimrep();
                  reenter();
                  break;
                case LeaveNotify:
                { int x,y; unsigned int w,h,b,d; Window ww;
                  XGetGeometry(disp(),win(),&ww,&x,&y,&w,&h, &b, &d);
                  dbx(1,"Geom: (%i,%i)+(%i,%i); b=%i d=%i",x,y,w,h,b,d); }
                  if (mml)
                    mml.top().tenterleave(false);
                  releaserep();
                  containspointer=false;
                  break;
                case Expose:
                  if (redraw)
                    redraw.top().tredraw(xev.xexpose.x,xev.xexpose.y,
                                         xev.xexpose.width,
                                         xev.xexpose.height);
                  break;
                case MotionNotify:
                  if (mml)
                    mml.top().tmotion(xev.xmotion.x,xev.xmotion.y);
                  break;
                case ClientMessage:
                  if (xev.xclient.message_type==wmprot &&
                      Atom(xev.xclient.data.l[0])==wmdelw)
                    quit();
                  break;
                }
              cbl.callpopall();
            }
        }
#ifdef NICE
      else if (!moresync && nextpoll>USLEEP_ADVERTISED_COMPENSATION)
        usleep(1); // could be 9999 == just under 1 cs.
#endif
    }
  }

/* ------------------------------- lastchar ------------------------------- */
int TEnv::lastchar() {
  const int BUFSIZE=8;
  char buf[BUFSIZE];
  KeySym ks;
  if (!lastkey)
    return 0;
  int len=XLookupString(lastkey,buf,BUFSIZE,&ks,0);
  if (len==1 && *buf>=32 && *buf!=127)
    return *buf;
  switch (ks)
    {
    case XK_BackSpace: return 8;
    case XK_Tab: return 9;
    case XK_Linefeed: case XK_Return: case XK_KP_Enter: return 13;
    case XK_Escape: case XK_Pause: case XK_Break: return 27;
    case XK_Delete: return 127;
    case XK_Left: return 256 + 8;
    case XK_Right: return 256 + 9;
    case XK_Down: return 256 + 10;
    case XK_Up: return 256 + 11;
    case XK_Home: return 512 + 8;
    case XK_End: return 512 + 9;
    case XK_Page_Down: return 512 + 10;
    case XK_Page_Up: return 512 + 11;
    }
  return 0;
  }


/* --------------------------------- quit --------------------------------- */
void TEnv::quit() {
  dbx(1,"TEnv::quit");
  dbx(3,"T:q popping quith's");
  while (quith)
    { THQuit *q=&quith.top();
      q->tquit();
      if (&quith.top()==q)
        quith.pop(q); // special care: the handler may pop itself!
    }
  quitting=true;
  }

TEnv::~TEnv() {
  dbx(1,"~TEnv");
  delete tcmap;
  releaserep();
  XCloseDisplay(display);
  }

/* ------------------------------ getautorep ------------------------------ */
bool TEnv::getautorep() {
  XKeyboardState ks;
  XGetKeyboardControl(display,&ks);
  return ks.global_auto_repeat==AutoRepeatModeOn;
  }

/* ---------------------------- do_setautorep ----------------------------- */
void TEnv::do_setautorep(bool a) {
  if (a)
    XAutoRepeatOn(display);
  else
    XAutoRepeatOff(display);
  }

void TEnv::claimrep() {
  if (repclaimed)
    return;
  extautorep=getautorep(); do_setautorep(wishautorep);
  repclaimed=true;
  }

void TEnv::releaserep() {
  if (!repclaimed)
    return;
  do_setautorep(extautorep);
  repclaimed=false;
  }

void TEnv::confine() {
  if (confined)
    athrow("TEnv: already confined");
  XRaiseWindow(display,window);
  fthrow(XGrabPointer(display,window,False,
                      ButtonPressMask|ButtonReleaseMask,
                      GrabModeAsync,GrabModeAsync,
                      window,None,CurrentTime)==GrabSuccess,
         "TEnv: could not grab pointer");
  confined=true;
  }

void TEnv::release() {
  if (!confined)
    athrow("TEnv: not confined");
  XUngrabPointer(display,CurrentTime);
  confined=false;
  }

string TEnv::id() const {
  char buf[12];
  sprintf(buf,"%ix%i",wid,hei);
  return string(buf);
  }

Point TEnv::pointer(bool *inme_ret) const {
  Window root,child;
  int rootx,rooty;
  int winx,winy;
  unsigned int mask;
  XQueryPointer(display,window,&root,&child,
                &rootx,&rooty,
                &winx,&winy,
                &mask);
  if (inme_ret)
    *inme_ret=(child==win());
  return Point(winx,winy);
  }

void TEnv::reg_mml(THMML *h) {
  if (containspointer)
    if (mml)
      mml.top().tenterleave(false);
  mml.push(h);
  reenter();
  }

void TEnv::dereg_mml(THMML *h) {
  mml.pop(h);
  reenter();
  }

void TEnv::reenter() {
  bool inme;
  Point p(pointer(&inme));
  if (inme)
    { if (mml)
        mml.top().tenterleave(true);
      if (mml)
        mml.top().tmotion(p.x(),p.y());
    }
  containspointer=inme;
  }

void TEnv::dereg_redraw(THRedraw *h) {
  redraw.pop(h); 
  if (redraw)
    redraw.top().tredraw(0,0,wid,hei);
  }

int TEnv::buttonmask(int button) {
  return button==Button1?Button1Mask
    :button==Button2?Button2Mask
    :button==Button3?Button3Mask
    :button==Button4?Button4Mask
    :button==Button5?Button5Mask
    :0;
  }

int TEnv::button(int button) {
  return button==Button1?1
    :button==Button2?2
    :button==Button3?3
    :button==Button4?4
    :button==Button5?5
    :0;
  }

void TEnv::getpixmapformat() { // get bpp and scanline padding for TImage
  int d=depth();
  int n;
  XPixmapFormatValues *p=XListPixmapFormats(display,&n);
  for (int i=0; i<n; i++)
    if (p[i].depth==d)
      { _bpp=p[i].bits_per_pixel;
        _scanlpad=p[i].scanline_pad;
        XFree(p);
        return;
      }
  athrow(string("TEnv: no ZPixmap format for present screen depth.\nDo you have an unusual X server, or am I being stupid?\nPlease mail me (wagenaar@mth.kcl.ac.uk) with xdpyinfo details."));
  }

void TEnv::reg_splayer(SPlayer *sp) {
  spl=sp;
  }

void TEnv::dereg_splayer(SPlayer *sp) {
  if (sp!=spl)
    athrow("TEnv::dereg_splayer: wrong SPlayer pointer");
  spl=0;
  }
