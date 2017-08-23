// GlobalOpts.H

#ifndef _GlobalOpts_H
#define _GlobalOpts_H

#include <string>
#include <vector>
#include <iostream>
#include "../basics/KeyCode.h"

using namespace std;

istream &operator>>(istream &is, class GlobalOpts &go);
ostream &operator<<(ostream &os, class GlobalOpts &go);

class GlobalOpts {
public:
  GlobalOpts() {
  }
  GlobalOpts(istream &is) {
    is >> *this;
  }
  vector<TKeyCode> const &keys(int pos) const {
    return pos < 0 ? keys_left : pos > 0 ? keys_right : keys_alone;
  }
protected:
  vector<TKeyCode> keys_alone, keys_left, keys_right;
  friend istream &operator>>(istream &is, GlobalOpts &go);
  friend ostream &operator<<(ostream &os, GlobalOpts const &go);
};

#endif