// InputField.H - a transparent input box

#ifndef _InputField_H
#define _InputField_H

const int REPEAT_RATE = 20; // ms
const int REPEAT_DELAY = 200; // ms
const int CURSOR_BLINK_DELAY = 500; // ms

#include <string>
using namespace std;

#include "../gfx/GBox.h"
#include "../env/TTime.h"
#include "../poll/Waker.h"
#include "../poll/Sleeper.h"

class InputField: public GBox, public Waker, public Sleeper {
public:
  enum { W_NONE=0, W_CHANGE, W_ENTER };
public:
  InputField(class GBParent *par, class Area const &area,
                 string const &initcont, class TFont &font,
                   class PollServer &pserv, class TEnv &env);
public:
  // from GBox
  virtual void redraw(class BBox const & bbox);
  virtual bool mouse(int button, bool ino, class Point const & xy);
  virtual bool key(int key, bool in_not_out);
  virtual bool offerfocus();
  virtual void forcedlosefocus();
public:
  // from Sleeper
  virtual void poll();   // for autorept and cursor
private:
  void key(int ascii);
  void cursoronoff(bool on=1, bool alarm=1);
  void drawrest();
  void recalcxpos();
private:
  int reptascii;   // what to repeat, zero if none
  TTime nextrept;   // when next to repeat
  string contents;
  unsigned int cpos;   // cursor position in characters
  int xpos;   // cursor position graphically, relative to start of string
  int baseliney;   // relative to our top().
  bool cursorstate;   // is cursor visible?
  TTime nextcurs;   // when next to toggle cursor
  class TFont &font;
  class TEnv &env;
};
#endif