// ListBox.H

#ifndef LISTBOX_H

#define LISTBOX_H

#include <string>
#include <vector>
using namespace std;

#include "../gfx/Box3D.h"

class ListBox: public Box3D {
public:
  ListBox(class GBParent *par, class QSize const &area,
              class TFont const &font, class TFont const &hlfont,
                  class BoxMarbler *bg);
  virtual ~ListBox();
  void add(string const &str);
  static int heightneeded(int n, class TFont const & font);
public:
  virtual void redraw(class BBox const & bbox);
  virtual void motion(class QPoint const & pt);
  virtual bool mouse(int button, bool ino, class QPoint const & pt);
  virtual void enterleave(bool ino, class QPoint const & pt);
protected:
  virtual void selection(int idx); // This gets called when a selection is made.
private:
  int liney(int n); // Converts line number to y position (not index to y), or -1
  int lineno(int y); // Converts y to line number (not to index), or -1
  int lineidx(int n); // Converts a line number to an index, or -1
  bool needsprev(int idx=-1);
  bool needsnext(int idx=-1);
  void nextpage();
  void prevpage();
private:
  vector<string> contents;
  int topidx;
  int numlines;
  int selectedline; // This may refer to prev or next!
  bool hasprev;
  bool hasnext;

  class TFont const & font;
  class TFont const & hlfont;
};

#endif