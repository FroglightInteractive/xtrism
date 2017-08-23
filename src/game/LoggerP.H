// private header for Logger.C and Replay.C
// Defines file format for Logger file:
// Note that the header is actually defined in Logger::write and
// Logger::Logger(FILE*), which must be kept in sync!

// +0 pitwidth (useful for learning algorithm, perhaps)
// +1 pitheight
// +2 type: 0=single, 1=team, 2=fight
// +3 num.of events
// +7 events
// ...

#ifndef _LoggerP_H
#define _LoggerP_H

#include <math.h>
#include "../basics/minmax.H"

const unsigned int LOGGER_SIZE = 10000; // prepare for 10000 events at a time

enum LTypes {
  LT_ADDBRICK=0,
  LT_REMBRICK,
  LT_PUDDING,
  LT_NEWNEXT,
  LT_QUIT=LT_NEWNEXT
};

struct LHeader {
  unsigned int time : 6;
  unsigned int type : 2;
};

struct LAddRemBrick {
  unsigned int bno : 6, rot : 2, x : 4, y : 6, def : 1;
  signed int id : 2;
};

struct LPudding {
  unsigned int y : 6, dir : 1;
};

struct LNewNext {
  unsigned int id : 1, bno : 6, rot : 2;
};

struct LQuit { // is LNewNext with bno=63.
  unsigned int alwaystrue : 9;
};

struct LEvent {
  LHeader header;
  union {
    LAddRemBrick addrembrick;
    LPudding pudding;
    LNewNext newnext;
    LQuit quit;
  };
};

class LNode {
public:
  LNode(unsigned int size0);
  ~LNode();   // deletes recursively!
  LEvent &event(unsigned int n);
  bool read(FILE *f);   // returns true if OK
  bool write(FILE *f, unsigned int length);   // returns true if OK. recurses!
private:
  class LNode *next;
  unsigned int size;
  LEvent *data;
};

inline unsigned int Logger_timerep(TTime const &now, TTime const &old) {
  int dt = now - old;
  unsigned int ldt = (unsigned int)(8 * log(dt + 1));
  return min(ldt, 63);
}

inline unsigned int Logger_invtimerep(unsigned int ldt) {
  return (unsigned int)(exp(ldt / 8.) - 1);
}

#endif