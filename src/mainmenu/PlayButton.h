// PlayButton.H

#ifndef _PlayButton_H
#define _PlayButton_H

#include "../gfx/Button.h"
#include "../globals/Globals.h"
#include "../sessions/SessionQuit.h"
#include "../env/TEnv.h"

class PlayButton: public TextButton, private SessionQuit, private TCallBack,
  private THQuit {
public:
  PlayButton(GBParent *p, const QSize &area, char const *txt,
             class TImage &bg0, bool bgok0):
    TextButton(p, area, txt, tf()), s(0),
    bg(bg0), bgok(bgok0) {
  }
  virtual ~PlayButton();
  virtual bool mouse(int b, bool io, const QPoint &p);
  virtual bool key(int kc, bool io);
  virtual void redraw(BBox const &bbox);
  virtual void play(int bset=-1) = 0;
  virtual void squit();   // called at session's end
  virtual void callback(int code=0);
  virtual void tquit();   // called when TEnv quitting
  virtual bool offerselect();
  virtual void forcedloseselect();
protected:
  void setsession(class NiceSession *s0);
  friend class SoloButton;
  friend class ApartButton;
  friend class TeamButton;
private:
  void visualselect(bool sel);
private:
  class NiceSession *s;
  class TImage &bg;
  bool bgok;
};

class SoloButton: public PlayButton {
public:
  SoloButton(GBParent *p, const QSize &area,
             bool right0, char const *txt,
             class TImage &bg, bool bgok):
    PlayButton(p, area, txt, bg, bgok), right(right0) {
  }
  void play(int bset=-1);
private:
  bool right;
};

class TeamButton: public PlayButton {
public:
  TeamButton(GBParent *p, const QSize &area, char const *txt,
             class TImage &bg, bool bgok):
    PlayButton(p, area, txt, bg, bgok) {
  }
  void play(int bset=-1);
};

class ApartButton: public PlayButton {
public:
  ApartButton(GBParent *p, const QSize &area, char const *txt,
              class TImage &bg, bool bgok):
    PlayButton(p, area, txt, bg, bgok) {
  }
  void play(int bset=-1);
};
#endif