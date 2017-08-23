// Controls.H

#ifndef _Controls_H
#define _Controls_H

class Controls {
public:
  Controls(class Keyboard *kbd);
  virtual ~Controls();
  virtual bool key(int /*code*/, bool /*in_not_out*/) {
    return false;
  }
private:
  class Keyboard *keybd;
};

#endif