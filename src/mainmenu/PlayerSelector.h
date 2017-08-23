// PlayerSelector.H

#ifndef PLAYERSELECTOR_H

#define PLAYERSELECTOR_H

#include "../gfx/Button.h"
#include "../globals/Globals.h"

class PlayerSelector: public TextButton {
public:
  PlayerSelector(GBParent *p, const Area &area, bool right,
                 class TImage &bg0, bool bgok0,
                   class BoxMarbler *marbler);
  virtual ~PlayerSelector();
  virtual bool mouse(int b, bool io, const Point &pt);
  virtual void redraw(BBox const &bb);
  void selection(int idx); // only to be called by PlayerListBox!
private:
  void openbox();
  void closebox();
private:
  GBParent *parent;
  class PlayerListBox *listbox;
  class TImage &bg;
  bool bgok;
  class BoxMarbler *marbler;
  bool right;
};

#endif