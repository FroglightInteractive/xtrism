// Controls.C

#include "Controls.h"

#include "Keyboard.h"

Controls::Controls(Keyboard *kbd): keybd(kbd) {
  keybd->reg(this);
}

Controls::~Controls() {
  keybd->dereg(this);
}