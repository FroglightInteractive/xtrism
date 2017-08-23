// Logger.H - makes a log of a game

#ifndef _Logger_H
#define _Logger_H

#include "FBPos.H"
#include "../basics/Byte.H"
#include "../env/TTime.H"
#include <stdio.h>

class Logger {
public:
  Logger(unsigned int pitwidth, unsigned int pitheight, byte type);
  Logger(FILE *f);   // read from file
  ~Logger();
  void write(FILE *f);

  void settime(TTime const &t) {
    time = t;
  }
  void addbrick(FBPos const &pos, int id, bool definitive);
  void rembrick(FBPos const &pos);
  void rembrick(int id);
  void pudding(int y, int dir);
  void newnext(int id, int bno, int rot);
  void quit();
  void pause(bool p);

  int pitwidth() {
    return pitwid;
  }
  int pitheight() {
    return pithei;
  }
  int type() {
    return typ;
  }
  class LNode *givedata() {
    return data;
  }                                          // for Replay only!
private:
  TTime time;
  TTime lasttime;
  class LNode *data;
  unsigned int latestoffset;
  byte pitwid, pithei, typ;
  bool paused;
  class LEvent &newevent(unsigned int type);
};
#endif