// MainMenu.H

#include "../gfx/TopBox.h"

class MainMenu: public TopBox {
public:
  MainMenu();
  ~MainMenu();
  void redraw(const BBox &bb);
public:
  // for the time being...
  class TextButton *playbuttons[4], *textbut;
private:
  class MMBG *backg;
  class BoxMarblers *bms;
  // class Box3D *boxje;
  class InputField *inpf;
  class PlayerSelector *selectors[2];
};