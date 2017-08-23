// stringtok.C

#include "stringtok.H"

string stringtok::token(char delim) {
  unsigned int a = str.find_first_not_of(delim, pos);
  if (a > str.max_size()) return "";

  pos = str.find_first_of(delim, a);
  return str.substr(a, pos - a);
}

string stringtok::token(char const *delims) {
  unsigned int a = str.find_first_not_of(delims, pos);
  if (a > str.max_size()) return "";

  pos = str.find_first_of(delims, a);
  return str.substr(a, pos - a);
}