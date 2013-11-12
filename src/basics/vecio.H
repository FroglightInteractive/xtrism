// vecio.H

#ifndef _vecio_H
#define _vecio_H

#include <iostream.h>
#include <backward/strstream>
#include <vector.h>
#include <string.h>
using namespace std;

template <class T> void operator>>(string const &s, vector<T> &v) {
  istrstream iss(s.c_str());
  while (iss.good())
    { T elt; iss >> elt;
      if (!iss.fail())
        v.push_back(elt);
    }
  }
template <class T> istream &operator>>(istream &is, vector<T> &v) {
  string s;
  getline(is,s);
  s >> v;
  return is;
  }

template <class T> ostream &operator<<(ostream &os, vector<T> const &v) {
  typename vector<T>::const_iterator i;
  for (i=v.begin(); i!=v.end(); ++i)
    os << *i << ' ';
  return os;
  }

#endif
