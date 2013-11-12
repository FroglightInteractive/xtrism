// PlayerList.H

#ifndef _PlayerList_H
#define _PlayerList_H

#include "Player.H"
#include <list>
#include <string>

class PlayerList {
  public:
  typedef list<Player>::iterator PlayerIt;
    PlayerList() { }
    PlayerList(istream &is) { is >> *this; }
    Player &operator[](int id);
    Player &operator[](string &name);
    PlayerIt begin() { return plrs.begin(); }
    PlayerIt next(PlayerIt i, bool loop=0);
    PlayerIt previous(PlayerIt i, bool loop=0);
    PlayerIt end() { return plrs.end(); }
    int count();
  protected:
    list<Player> plrs;
    friend istream &operator>>(istream &is, PlayerList &pll);
  };

istream &operator>>(istream &is, PlayerList &pll);

#endif
