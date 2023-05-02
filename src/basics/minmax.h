// minmax.H

#ifndef MINMAX_H

#define MINMAX_H

#include <algorithm>

template <class X> unsigned char clip255(X const &x) {
  return x<0 ? 0 : x>255 ? 255 : (unsigned char)x;
}

#endif
