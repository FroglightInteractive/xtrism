// Button.H

#ifndef _Button_H
#define _Button_H

#include <string>
#include "GBParent.H"
#include "../env/TDrawCol.H"
#include "../env/TFont.H"
#include <stdio.h>

class Button: public GBox {
  public:
    Button(GBParent *p, Area const &area, const TRGB &bg);
    Button(GBParent *p, Area const &area);
    Button(GBParent *p, class TImage const &image);
    virtual ~Button();
    virtual void redraw(const BBox &bbox);
    virtual bool mouse(int b, bool io, const Point &p)
      { printf("Button: Click! %i/%i (%i,%i) (this=%p)\n",
               b,io,p.x(),p.y(),this); return true; }
    virtual void enterleave(bool io, const Point &p)
      { printf("Button: enterleave(%i) (this=%p)\n",int(io),this); }
  private:
    union {
        TDrawCol *tdc;
        TImage const *img;
      };
    enum { COLOUR, IMAGE, TRANSP } type;
  };

class TextButton: public Button {
  public:
    TextButton(GBParent *p, const Area &area,
               const TRGB &bg, char *txt, const TFont &tf):
      Button(p,area,bg), text(txt), font(tf) {}
    TextButton(GBParent *p, const Area &area,
               char *txt, const TFont &tf):
      Button(p,area), text(txt), font(tf) {}
    virtual void redraw(const BBox &bbox)
      { Button::redraw(bbox);
        font.write(text.c_str(),left()+width()/2,
                   top()+(height()+font.xheight())/2,TFont::CENTRE);
      }
    virtual bool mouse(int b, bool io, const Point &p)
      { printf("TextButton: Click! %i/%i (%i,%i) (this=%p)\n",
               b,io,p.x(),p.y(),this); return true; }
    void settext(string const &s) { text=s; redraw(bbox()); }
    string const &gettext() const { return text; }
  private:
    string text;
    const TFont &font;
  };

#endif
