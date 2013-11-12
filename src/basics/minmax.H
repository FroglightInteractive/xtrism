// minmax.H

#ifndef MINMAX_H

#define MINMAX_H

#include <algorithm>
using namespace std;

template <class X> void makeatleast(X &a, X const &b) {
  if (a<b)
    a=b;
}

template <class X> void makeatmost(X &a, X const &b) {
  if (a>b)
    a=b;
}


#endif
