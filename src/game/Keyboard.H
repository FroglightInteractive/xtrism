// Keyboard.H

#ifndef _Keyboard_H
#define _Keyboard_H

#include <set>
using namespace std;

class Keyboard {
  public:
    Keyboard() {}
    void reg(class Controls *c) { ctrls.insert(c); }
    void dereg(class Controls *c) { ctrls.erase(c); }
    bool key(int code, bool in_not_out);
  private:
    typedef set<class Controls *,less<class Controls *> > CtrlSet;
    CtrlSet ctrls;
  };

#endif
