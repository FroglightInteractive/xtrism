// TFont.C

#define TPRIVATE

#include "../basics/Throw.H"
#include "TFont.H"
#include "../basics/BBox.H"
#include "setclip.H"

#include <stdio.h>

////////////////////////////// TMFont ////////////////////////////////////////
TMFont::TMFont(const TGFX &tgfx0, char *name, int size):
  tgfx(tgfx0) {
  if (size)
    { char buf[80];
      sprintf(buf,name,size);
      name=buf;
    }
  font=XLoadQueryFont(tgfx.disp(),name);
  fthrow(font,"TFont: cannot find font");
  }

TMFont::~TMFont() {
  if (font)
    XFreeFont(tgfx.disp(),font);
  }

//////////////////////////////// TFont ///////////////////////////////////////
TFont::TFont(const TMFont &tmf, const TRGB &fg, const TRGB &bg):
  tgfx(tmf.tgfx), font(tmf.font),tdc(tmf.tgfx,bg) {
  XGCValues gcv;
  gcv.font=font->fid;
  gcv.foreground=tgfx.cmap()(fg);
  gcv.background=tgfx.cmap()(bg);
  gc=XCreateGC(tgfx.disp(),tgfx.win(),GCForeground|GCBackground|GCFont,&gcv);
  ycor=0; /* (font->per_char['X'-font->min_char_or_byte2].ascent -
        font->per_char['g'-font->min_char_or_byte2].descent)/2; */
  }

TFont::~TFont() {
  if (gc)
    XFreeGC(tgfx.disp(),gc);
  }

int TFont::stringwidth(char const *string) const {
  int direction, ascent, descent;
  XCharStruct overall;
  XTextExtents(font,string,strlen(string),
               &direction, &ascent, &descent,
               &overall);
  return overall.width;
  }  

void TFont::write(char const *string, int x, int y,
                  TFAlign align, bool bg, int totalw) const {
  int sw=stringwidth(string);
  switch (align)
    {
    case LEFT: break;
    case CENTRE: x-=sw/2; break;
    case RIGHT: x-=sw; break;
    }
  y+=ycor;
  if (bg)
    { XDrawImageString(tgfx.disp(),tgfx.win(),gc,x,y,string,strlen(string));
      int dw=totalw-sw;
      if (dw>0)
        switch (align)
          {
          case LEFT: clearrect(x+sw,y,dw); break;
          case RIGHT: clearrect(x-dw,y,dw); break;
          case CENTRE: clearrect(x+sw,y,dw/2); clearrect(x-dw/2,y,dw/2);
            break;
          }
    }
  else
    XDrawString(tgfx.disp(),tgfx.win(),gc,x,y,string,strlen(string));
  }

int TFont::ascent() const {
  return font->ascent;
  }

int TFont::descent() const {
  return font->descent;
  }

int TFont::xheight() const {
  return font->per_char['x'-font->min_char_or_byte2].ascent;
  }

void TFont::clearrect(int x, int y, int w) const {
  tdc.rectangle(x,y-ascent(),w,height(),true);
  }

void TFont::setclip(BBox const &bb) {
  ::setclip(tgfx,gc,bb);
  }
