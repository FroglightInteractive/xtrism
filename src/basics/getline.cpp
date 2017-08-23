#include <string>
#include <iostream>

using namespace std;

#include "dbx.h"

const int GL_LLEN = 1000;

static char gl_linebuf[GL_LLEN];

string getline_nocmt(istream &is) {
  string str;
  string cmt("%#!;/");
  while (is.good()) {
    is.getline(gl_linebuf, GL_LLEN);
    str = gl_linebuf;
    if (cmt.find(str[0]) >= cmt.max_size() && str.length())
      return str;
  }
  return "";
}

void skiptoeol(istream &is) {
  char ch = 0;
  while (is.good() && ch != '\n')
    is.get(ch);
}

void skipcmt(istream &is) {
  char ch;
  string cmt("%#!;/");
  while (is.good()) {
    is.get(ch);
    if (cmt.find(ch) < cmt.max_size())
      skiptoeol(is);
    else if (ch > 32) {
      if (!is.fail())
        is.putback(ch);
      break;
    }
  }
}

istream &skipwhite(istream &is) {
  char c;
  do is.get(c);
  while (c == ' ' || c == '\t');
  if (is.good())
    is.putback(c);
  return is;
}