// Player.H

#ifndef _Player_H
#define _Player_H

#include <string>
#include <vector>
#include <iostream>
using namespace std;

#include "../basics/KeyCode.h"

istream &operator>>(istream &is, class Player &pl);
ostream &operator<<(ostream &os, class Player const &pl);

class Player {
public:
  Player(): id_(0) {
  }
  Player(istream &is): id_(0) {
    is >> *this;
  }

  int id() const {
    return id_;
  }
  string const &name() const {
    return name_;
  }
  vector<TKeyCode> const &keys(int where /* in {-1,0,1} */) const;
  short level(unsigned int idx) const;
  int nextpos() const {
    return next_left ? -1 : 1;
  }
  bool dwxpos() const {
    return dw_xpos;
  }
  Player &operator=(Player const &p);
protected:
  int id_;
  string name_;
  vector<TKeyCode> keys_alone, keys_left, keys_right;
  bool next_left;
  bool dw_xpos;
  vector<short> level_;
  friend istream &operator>>(istream &is, Player &pl);
  friend ostream &operator<<(ostream &os, Player const &pl);
};

#endif