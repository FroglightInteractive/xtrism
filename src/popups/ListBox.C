// ListBox.C

#include "ListBox.H"

#include "../basics/dbx.H"
#include "../env/TFont.H"

#define LEFTMARGIN 10
#define RIGHTMARGIN 10
#define TOPMARGIN 6
#define BOTTOMMARGIN 6
#define LEADING int(font.height() * 1.2)
#define YSHIFT (TOPMARGIN + font.ascent())

int ListBox::heightneeded(int n, TFont const &font) {
  return TOPMARGIN + LEADING * n + BOTTOMMARGIN;
}

int ListBox::liney(int n) {
  if (n < 0 || n >= numlines)
    return -1;

  return YSHIFT + n * LEADING + top();
}

int ListBox::lineno(int y) { // This does *not*
  int n = (y - TOPMARGIN - top()) / LEADING;
  if (n < 0 || n > numlines)
    return -1;
  else
    return n;
}

int ListBox::lineidx(int n) {
  if (n < 0)
    return -1;

  if (hasprev)
    n--;
  n -= topidx;
  if (n >= 0 && n < int(contents.size()))
    return n;
  else
    return -1;
}

bool ListBox::needsprev(int idx) {
  if (idx < 0)
    idx = topidx;
  return idx > 0;
}

bool ListBox::needsnext(int idx) {
  if (idx < 0)
    idx = topidx;
  return idx + numlines - (needsprev(idx) ? 1 : 0) < int(contents.size());
}

ListBox::ListBox(class GBParent *par, class Area const &area,
                     class TFont const &font0, class TFont const &hlfont0,
                         class BoxMarbler *bg):
  Box3D(par, area, bg), font(font0), hlfont(hlfont0) {
  hasnext = hasprev = false;
  topidx = 0;
  numlines = int((area.h() - TOPMARGIN - BOTTOMMARGIN) / LEADING);
  selectedline = -1;
}

ListBox::~ListBox() {
}

void ListBox::add(string const &str) {
  contents.push_back(str);
  if (!hasnext)
    hasnext = needsnext();
}

void ListBox::redraw(BBox const &bb) {
  Box3D::redraw(bb);
  // Really, perhaps I ought to set the TFont clip, but I won't
  if (hasprev) {
    int wid = font.stringwidth("Previous");
    BBox box(right() - RIGHTMARGIN - wid, top() + TOPMARGIN,
             right() - RIGHTMARGIN, top() + TOPMARGIN + LEADING);
    if (bb.intersect(box)) {
      // Must redraw the word "Previous"
      ((selectedline == 0) ? hlfont : font).write("Previous",
                                                  right() - RIGHTMARGIN - wid,
                                                  top() + YSHIFT);
    }
  }
  if (hasnext) {
    int wid = font.stringwidth("Next");
    BBox box(right() - RIGHTMARGIN - wid,
             top() + TOPMARGIN + (numlines - 1) * LEADING,
             right() - RIGHTMARGIN, top() + TOPMARGIN + numlines *LEADING);
    if (bb.intersect(box)) {
      // Must redraw the word "Next"
      ((selectedline == numlines - 1) ? hlfont : font).write("Next",
                                                             right() - RIGHTMARGIN - wid,
                                                             top() + YSHIFT
                                                             + (numlines - 1)
                                                             * LEADING);
    }
  }
  for (int n = (hasprev ? 1 : 0); n < numlines - (hasnext ? 1 : 0); n++) {
    int idx = n + topidx;
    dbx(-20070110, "listbox redraw n=%i idx=%i", n, idx);
    if (idx >= int(contents.size()))
      break;
    BBox box(left() + LEFTMARGIN, top() + TOPMARGIN + n *LEADING,
             right() - RIGHTMARGIN, top() + TOPMARGIN + (n + 1) * LEADING);
    if (bb.intersect(box)) {
      // Must redraw a line
      ((selectedline == n) ? hlfont : font).write(contents[idx].c_str(),
                                                  left() + LEFTMARGIN,
                                                  top() + YSHIFT + n
                                                  * LEADING);
    }
  }
}

void ListBox::motion(Point const &pt) {
  // dbx(-20070110,"listbox(%p)::motion((%i,%i))",
  // this,pt.x(),pt.y());
  int n = lineno(pt.y());
  if (n != selectedline) {
    selectedline = n;
    redraw(bbox());
  }
}

bool ListBox::mouse(int button, bool ino, Point const &pt) {
  // dbx(-20070110,"listbox(%p)::mouse(%i,%i,(%i,%i))",
  // this,button,ino?1:0,pt.x(),pt.y());
  if (button == 1 && ino) {
    if (hasprev && selectedline == 0) {
      prevpage();
    } else if (hasnext && selectedline == numlines - 1) {
      nextpage();
    } else {
      int idx = lineidx(selectedline);
      selection(idx);
    }
    return true;
  }
  return false;
}

void ListBox::prevpage() {
  int step = numlines - 1; // By definition, we'll have a "Next"
  if (needsprev(topidx - step))
    step--; // We'll also have a "Prev"
  topidx -= step;
  if (topidx < 0)
    topidx = 0;
  hasnext = needsnext();
  hasprev = needsprev();
  redraw(bbox());
}

void ListBox::nextpage() {
  int step = numlines - 1; // By definition, we had a "Next"
  if (hasprev)
    step--; // We'll also have a prev
  topidx += step;
  if (topidx >= contents.size() - (numlines - 1))
    topidx = contents.size() - (numlines - 1);
  hasnext = needsnext();
  hasprev = needsprev();
  redraw(bbox());
}

void ListBox::selection(int idx) {
  dbx(-20070110, "Listbox selection: %i", idx);
}

void ListBox::enterleave(bool ino, Point const &pt) {
}