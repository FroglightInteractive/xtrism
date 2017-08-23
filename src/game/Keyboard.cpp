// Keyboard.C

#include "Keyboard.h"
#include "Controls.h"

bool Keyboard::key(int code, bool in_not_out) {
  CtrlSet::iterator i;
  for (i = ctrls.begin(); i != ctrls.end(); ++i)
    if ((*i)->key(code, in_not_out))
      return true;

  return false;
}