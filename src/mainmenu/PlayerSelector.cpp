// PlayerSelector.C

#include "PlayerSelector.h"

#include "../popups/ListBox.h"
#include "../env/TEnv.h"
#include "../basics/minmax.h"
#include "../pics/MarbleBG.h"
#include "../env/TImage.h"
#include "../options/PlayerList.h"

class PlayerListBox: public ListBox {
public:
  PlayerListBox(class GBParent *par, class QSize const &area,
                    class TFont const &font, class TFont const &hlfont,
                        class BoxMarbler *bg, PlayerSelector *selector0):
    ListBox(par, area, font, hlfont, bg), selector(selector0) {
  }
  virtual ~PlayerListBox() {
  }
  virtual void selection(int idx) {
    if (selector)
      selector->selection(idx);
  }
private:
  PlayerSelector *selector;
};

PlayerSelector::PlayerSelector(GBParent *p, const QSize &area, bool right0,
                               class TImage &bg0, bool bgok0,
                                 class BoxMarbler *marbler):
  TextButton(p, area, "", tf()),
  parent(p), bg(bg0), bgok(bgok0), right(right0) {
  listbox = 0;
}

PlayerSelector::~PlayerSelector() {
  if (listbox)
    delete listbox;
  listbox = 0;
}

void PlayerSelector::redraw(BBox const &bbox) {
  if (!bgok) {
    // have to create our actual background.
    BBox const &mybb = this->bbox();
    int wid = bg.width(), hei = bg.height();
    marblebg(width(), height(), 2, // should be env.size dep.
             float(mybb.left()) / wid, float(mybb.top()) / hei,
             float(mybb.right()) / wid, float(mybb.bottom()) / hei,
             bg, mybb.left(), mybb.top());
    bgok = true;
    bg.partput(bbox, bbox.topleft()); // we can't be transp. the 1st time
  }
  TextButton::redraw(bbox);
}

bool PlayerSelector::mouse(int b, bool io, const QPoint &pt) {
  if (b == 1 && io) {
    openbox();
    return true;
  }
  return false;
}

void PlayerSelector::openbox() {
  if (listbox)
    return;

  int nplrs = players().count();
  int yneed = ListBox::heightneeded(nplrs, tf());
  makeatmost(yneed, int(3 * tenv().height() / 4));
  dbx(-20070110, "PlayerSelector: openbox. yneed=%i. nplrs=%i", yneed, nplrs);
  listbox = new PlayerListBox(parent, QSize(width(), yneed),
                              tf(), tfyellow(), marbler, this);
  for (PlayerList::PlayerIt i = players().begin();
       i != players().end(); ++i) {
    dbx(-20070110, "PlayerSelector: adding player %s", (*i).name().c_str());
    listbox->add((*i).name());
  }
  parent->newchild(listbox, GBPos(this, 1), GBPos(this, 1),
                   GBPos(this, 1), GBPos(parent, 10));
  dbx(-20070110, "PlayerSelector: added child to parent");
  parent->placechildren();
  dbx(-20070110, "PlayerSelector: children placed to parent");
}

void PlayerSelector::closebox() {
  dbx(-20070110, "PlayerSelector: closebox");
  if (listbox) {
    delete listbox;
    listbox = 0;
  }
}

void PlayerSelector::selection(int idx) {
  if (idx > 0) {
    for (PlayerList::PlayerIt i = players().begin();
         i != players().end(); ++i) {
      --idx;
      if (idx == 0) {
        setplayer(right, i);
        break;
      }
    }
  }
  closebox();
}