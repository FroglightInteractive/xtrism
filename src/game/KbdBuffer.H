// KbdBuffer.H

#ifndef _KbdBuffer_H
#define _KbdBuffer_H

#include <vector>
#include "../basics/KeyCode.H"
#include "../poll/Waker.H"
using namespace std;

enum KeyNumber {
  KN_None=-1,
  KN_First=0,
  KN_Left=0,
  KN_Right,
  KN_RotCW,
  KN_RotCCW,
  KN_Drop,
  KN_Zap,
  KN_MaxUser,
  
  KN_FirstGlobal=KN_MaxUser,
  KN_Pause=KN_FirstGlobal,
  KN_LevUp,
  KN_LevDn,
  KN_Quit,
  KN_Max,
  };

enum BufferCode {
  BC_None=0,
  BC_Out=16,
  BC_iLeft=1,
  BC_iRight,
  BC_iRotCW,
  BC_iRotCCW,
  BC_iDrop,
  BC_iZLeft,
  BC_iZRight,
  BC_iZap,
  BC_iPause,
  BC_iLevUp,
  BC_iLevDn,
  BC_iQuit,
  BC_oLeft=BC_Out+1,
  BC_oRight,
  BC_oRotCW,
  BC_oRotCCW,
  BC_oDrop,
  BC_oZLeft,
  BC_oZRight,
  BC_oZap,
  BC_oPause,
  BC_oLevUp,
  BC_oLevDn,
  BC_oQuit,
  };

enum BufferState {
  BS_Left=1<<KN_Left,
  BS_Right=1<<KN_Right,
  BS_RotCW=1<KN_RotCW,
  BS_RotCCW=1<<KN_RotCCW,
  BS_Drop=1<<KN_Drop,
  BS_Zap=1<<KN_Zap,
  BS_Pause=1<<KN_Pause,
  BS_LevUp=1<<KN_LevUp,
  BS_LevDn=1<<KN_LevDn,
  BS_Quit=1<<KN_Quit,
  };

class KeyCodes {
  public:
    KeyCodes(TKeyCode const *kcs);
    KeyCodes(vector<TKeyCode> const &kcs);
    KeyCodes(TKeyCode const *user, TKeyCode const *global);
    KeyCodes(vector<TKeyCode> const &user, vector<TKeyCode> const &global);
    KeyCodes(TKeyCode const *user, vector<TKeyCode> const &global);
    TKeyCode operator[](KeyNumber k) { return data[k]; }
    void setuser(TKeyCode const *user);
    void setuser(vector<TKeyCode> const &user);
    void setglobal(TKeyCode const *global);
    void setglobal(vector<TKeyCode> const &global);
  private:
    TKeyCode data[KeyNumber(KN_Max)];
  };

/* #define KB_BUFSIZE 40

class LimitedInt {
  public:
    LimitedInt(int value,int limit): val(value), lim(limit) {}
    LimitedInt(int limit): val(0), lim(limit) {}
    LimitedInt &inc() { val++; if (val==lim) val=0; return *this; }
    bool operator==(const LimitedInt &other) const { return val==other.val; }
    operator int() { return val; }
  private:
    int val;
    int lim;
  };
 */

#include <list>

class KbdBuffer: public Waker {
  public:
    KbdBuffer(TKeyCode const *usr, TKeyCode const *glb):
      kcs(usr,glb), // rp(KB_BUFSIZE), wp(KB_BUFSIZE),
      state(BufferState(0)), last(KN_None) { }
    KbdBuffer(vector<TKeyCode> const &usr, vector<TKeyCode> const &glb):
      kcs(usr,glb), // rp(KB_BUFSIZE), wp(KB_BUFSIZE),
      state(BufferState(0)), last(KN_None) { }
    KbdBuffer(TKeyCode const *user, vector<TKeyCode> const &glb):
      kcs(user, glb), // rp(KB_BUFSIZE), wp(KB_BUFSIZE),
      state(BufferState(0)), last(KN_None) { }
    bool enter(TKeyCode kc, bool in_not_out);
    // enter() returns 0 if not entered because code is unknown
    BufferCode read();
    void putback(BufferCode b);
  private:
    KeyCodes kcs;
    /* LimitedInt rp;
    LimitedInt wp; */
    BufferState state;
    list<BufferCode> data;

    void enter(BufferCode b);
    void ithreekey(KeyNumber nw);
    void othreekey(KeyNumber nw);
    KeyNumber last;
  };

#endif
