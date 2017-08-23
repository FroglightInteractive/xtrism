// Logger.C

#include "Logger.H"
#include "LoggerP.H"
#include <math.h>
#include "../basics/Throw.H"

//////////////////////////////////// LNode ///////////////////////////////////

LNode::LNode(unsigned int size0) {
  next = 0;
  size = size0;
  data = new LEvent[size];
}

LNode::~LNode() {
  if (next)
    delete next;
  delete[] data;
}

LEvent &LNode::event(unsigned int n) {
  if (n < size)
    return data[n];

  if (next)
    return next->event(n - size);

  if (n > size)
    athrow("LNode::event: can't access way beyond end of chain");
  next = new LNode(size); // automatic extension
  return next->event(0);
}

bool LNode::read(FILE *f) {
  return fread(data, sizeof(LEvent), size, f) == size;
}

bool LNode::write(FILE *f, unsigned int length) {
  unsigned int l0 = min(length, size);
  if (fwrite(data, sizeof(LEvent), l0, f) != l0)
    return false;

  if (length > size)
    return next ? (next->write(f, length - size)) : false;
  else
    return true;
}

/////////////////////////////////// Logger ///////////////////////////////////

Logger::Logger(unsigned int pitwidth0, unsigned int pitheight0, byte typ0):
  time(TTime::CURRENT), lasttime(TTime::CURRENT),
  pitwid(pitwidth0), pithei(pitheight0), typ(typ0) {
  data = new LNode(LOGGER_SIZE);
  latestoffset = 0;
}

Logger::Logger(FILE *f):
  time(TTime::CURRENT), lasttime(TTime::CURRENT) {
  // keep in sync with Logger::write() and the text in LoggerP.H
  fread(&pitwid, 1, 1, f);
  fread(&pithei, 1, 1, f);
  fread(&typ, 1, 1, f);
  fread(&latestoffset, sizeof(unsigned int), 1, f);
  data = new LNode(latestoffset);
  fthrow(data->read(f), "Logger(FILE): failed to read data");
}

Logger::~Logger() {
  delete data;
}

inline LEvent &Logger::newevent(unsigned int type) {
  LEvent &lev = data->event(latestoffset++);
  lev.header.time = Logger_timerep(time, lasttime);
  lev.header.type = type;
  return lev;
}

void Logger::write(FILE *f) {
  // keep in sync with Logger::Logger(FILE*) and the text in LoggerP.H
  fwrite(&pitwid, 1, 1, f);
  fwrite(&pithei, 1, 1, f);
  fwrite(&typ, 1, 1, f);
  fwrite(&latestoffset, sizeof(unsigned int), 1, f);
  fthrow(data->write(f, latestoffset), "Logger::write: failed to write data");
}

void Logger::addbrick(FBPos const &pos, int id, bool definitive) {
  if (paused)
    return;

  LEvent &lev = newevent(LT_ADDBRICK);
  lev.addrembrick.bno = pos.bno;
  lev.addrembrick.rot = pos.rot;
  lev.addrembrick.x = pos.x;
  lev.addrembrick.y = pos.y;
  lev.addrembrick.def = definitive;
  lev.addrembrick.id = id;
}

void Logger::rembrick(FBPos const &pos) {
  if (paused)
    return;

  LEvent &lev = newevent(LT_REMBRICK);
  lev.addrembrick.bno = pos.bno;
  lev.addrembrick.rot = pos.rot;
  lev.addrembrick.x = pos.x;
  lev.addrembrick.y = pos.y;
  lev.addrembrick.def = 0;
  lev.addrembrick.id = -1;
}

void Logger::rembrick(int id) {
  if (paused)
    return;

  LEvent &lev = newevent(LT_REMBRICK);
  lev.addrembrick.id = id;
}

void Logger::pudding(int y, int dir) {
  if (paused)
    return;

  LEvent &lev = newevent(LT_PUDDING);
  lev.pudding.y = y;
  lev.pudding.dir = (dir > 0);
}

void Logger::newnext(int id, int bno, int rot) {
  if (paused)
    return;

  LEvent &lev = newevent(LT_NEWNEXT);
  lev.newnext.id = id;
  lev.newnext.bno = bno;
  lev.newnext.rot = rot;
}

void Logger::quit() {
  LEvent &lev = newevent(LT_QUIT);
  lev.quit.alwaystrue = ~0u;
}

void Logger::pause(bool p) {
  paused = p;
}