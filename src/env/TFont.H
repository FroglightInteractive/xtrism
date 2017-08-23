// TFont.H - part of Trism's environment

#ifndef _TFont_H
#define _TFont_H

#include "TGFX.H"
#include "TDrawCol.H"

#ifndef TPRIVATE
#include "TXID.H"
#endif

class TMFont {
public:
  TMFont(const TGFX &tgfx, char *name, int size=0);
  ~TMFont();
private:
  const TGFX &tgfx;   // read by TFont
private:
  // device dependent
#ifdef TPRIVATE
  XFontStruct * font;  // read by TFont
#else
  void *font;
#endif
  friend class TFont;
};

enum TFAlign { TF_Left, TF_Centre, TF_Right };

class TFont {
public:
  enum TFAlign { LEFT, CENTRE, RIGHT };
  TFont(const TMFont &tmf, const TRGB &fg, const TRGB &bg);
  int stringwidth(char const *string) const;
  void write(char const *string, int x, int y, TFAlign align=LEFT,
             bool bg=false, int totalw=0) const;
  void writebg(char const *string, int x, int y,
               TFAlign align=LEFT, int totalw=0) const {
    write(string, x, y, align, true, totalw);
  }
  ~TFont();
  int ascent() const;
  int descent() const;
  int height() const {
    return ascent() + descent();
  }
  int xheight() const;
  TDrawCol const &backg() const {
    return tdc;
  }
  void setclip(class BBox const & bb);
private:
  void clearrect(int x, int y, int w) const;
private:
  // dev.dep.
private:
  TGFX const &tgfx;
#ifdef TPRIVATE
  GC gc;
  XFontStruct *font;
  int ycor;
#else
  TXID gc;
  void *font;
  int ycor;
#endif
  TDrawCol tdc;
};

#endif