// ControlBuf.H

#ifndef _ControlBuf_H
#define _ControlBuf_H

#include "KbdBuffer.h"
#include "Controls.h"

class ControlBuf: public Controls {
public:
  ControlBuf(class Keyboard *kbd, class KbdBuffer *buf):
    Controls(kbd), buffer(buf) {
  }
  bool key(int code, bool in_not_out) {
    return buffer->enter(code, in_not_out);
  }
private:
  KbdBuffer *buffer;
};

#endif