// PlayerList.C

#include "PlayerList.H"
#include "../basics/getline.H"
#include "../basics/Throw.H"
#include "../basics/dbx.H"

list<Player>::iterator PlayerList::next(list<Player>::iterator i,
                                        bool loop) {
  ++i;
  return (loop && i == end()) ? begin() : i;
}

list<Player>::iterator PlayerList::previous(list<Player>::iterator i,
                                            bool loop) {
  if (loop && i == begin())
    i = end();
  return --i;
}

Player &PlayerList::operator[](int id) {
  list<Player>::iterator i;
  for (i = plrs.begin(); i != plrs.end(); ++i)
    if ((*i).id() == id)
      return *i;

  athrow("PlayerList: unknown player ID");
}

Player &PlayerList::operator[](string &name) {
  list<Player>::iterator i;
  for (i = plrs.begin(); i != plrs.end(); ++i)
    if ((*i).name() == name)
      return *i;

  athrow("PlayerList: unknown player name");
}

istream &operator>>(istream &is, PlayerList &pll) {
  dbx(2, "istream >> PlayerList");
  while (is.good()) {
    skipcmt(is);
    if (is.good()) {
      dbx(3, "Creating a player");
      pll.plrs.push_back(Player());
      dbx(3, "Reading player data");
      is >> *pll.previous(pll.end());
      dbx(3, "Player created OK");
    }
  }
  dbx(3, "istr >> PlrL done");
  return is;
}

int PlayerList::count() {
  int n = 0;
  for (PlayerIt i = plrs.begin(); i != plrs.end(); ++i)
    n++;
  return n;
}