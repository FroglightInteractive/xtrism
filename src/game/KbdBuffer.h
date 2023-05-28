// KbdBuffer.H

#ifndef _KbdBuffer_H
#define _KbdBuffer_H

#include "GameKeys.h"
#include "MetaKeys.h"

enum KeyNumber {
  KN_None=-1,
  KN_Left=0,
  KN_Right,
  KN_RotCW,
  KN_RotCCW,
  KN_Drop,
  KN_Zap,
  KN_FloatDown,

  KN_Pause,
  KN_LevUp,
  KN_LevDn,
  KN_Quit,
  KN_Max,
};

enum BufferCode {
  BC_None=0,
  BC_iLeft,
  BC_iRight,
  BC_iRotCW,
  BC_iRotCCW,
  BC_iDrop,
  BC_iZLeft,
  BC_iZRight,
  BC_iZap,
  BC_iFloatDown,
  BC_iPause,
  BC_iLevUp,
  BC_iLevDn,
  BC_iQuit,
  BC_oLeft,
  BC_oRight,
  BC_oRotCW,
  BC_oRotCCW,
  BC_oDrop,
  BC_oZLeft,
  BC_oZRight,
  BC_oZap,
  BC_oFloatDown,
  BC_oPause,
  BC_oLevUp,
  BC_oLevDn,
  BC_oQuit,
};

enum BufferState {
  BS_Left=1<<KN_Left,
  BS_Right=1<<KN_Right,
  BS_RotCW=1 < KN_RotCW,
  BS_RotCCW=1<<KN_RotCCW,
  BS_Drop=1<<KN_Drop,
  BS_Zap=1<<KN_Zap,
  BS_FloatDown = 1<<KN_FloatDown,
  BS_Pause=1<<KN_Pause,
  BS_LevUp=1<<KN_LevUp,
  BS_LevDn=1<<KN_LevDn,
  BS_Quit=1<<KN_Quit,
};

class KeyCodes: public QMap<int, KeyNumber> {
public:
  KeyCodes(GameKeys const &gk, MetaKeys const &mk);
  void setuser(GameKeys const &user);
  void setglobal(MetaKeys const &global);
private:
  void rebuild();
private:
  GameKeys gk;
  MetaKeys mk;
};

#include <list>

class KbdBuffer {
public:
  KbdBuffer(GameKeys const &gk, MetaKeys const &mk);
  bool enter(int kc, bool in_not_out);
  // enter() returns 0 if not entered because code is unknown
  BufferCode read();
  void putback(BufferCode b);
  QList<int> allKeyCodes() const;
private:
  KeyCodes kcs;
  /* LimitedInt rp;
     LimitedInt wp; */
  BufferState state;
  std::list<BufferCode> data;

  void enter(BufferCode b);
  void ithreekey(KeyNumber nw);
  void othreekey(KeyNumber nw);
  KeyNumber last;
};

#endif
