// stringtok.H

#ifndef _stringtok_H
#define _stringtok_H

#include <string>
using namespace std;

class stringtok {
public:
  stringtok(): pos(0) {
  }
  stringtok(const string &s): str(s), pos(0) {
  }
  stringtok &operator=(const string &s) {
    str = s;
    pos = 0;
    return *this;
  }
  string token(char delim);
  string token(char const *delims=" \t");
private:
  string str;
  unsigned int pos;
};
#endif