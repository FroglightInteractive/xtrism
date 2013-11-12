// Controls.C

#include "Controls.H"

#include "Keyboard.H"

Controls::Controls(Keyboard *kbd): keybd(kbd) {
  keybd->reg(this);
  }

Controls::~Controls() {
  keybd->dereg(this);
  }
