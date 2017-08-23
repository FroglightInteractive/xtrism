// Filename.H

#ifndef _Filename_H
#define _Filename_H

#include <string>
using namespace std;

// Unix implementation:
#include <sys/stat.h>
#include <unistd.h>

const char Fn_DirSep = '/';
const char Fn_SufSep = '.';
const char Fn_PartSep = '-';

class Filename {
public:
  Filename(const string &base): nam(base) {
  }
  Filename() {
  }
  Filename &operator+=(const string &s) {
    nam += Fn_DirSep;
    nam += s;
    return *this;
  }
  Filename &operator+=(char const *s) {
    nam += Fn_DirSep;
    nam += s;
    return *this;
  }
  Filename &addsuffix(const string &s) {
    nam += Fn_SufSep;
    nam += s;
    return *this;
  }
  Filename &addsuffix(char const *s) {
    nam += Fn_SufSep;
    nam += s;
    return *this;
  }
  Filename &addpart(const string &s) {
    nam += Fn_PartSep;
    nam += s;
    return *this;
  }
  Filename &addpart(char const *s) {
    nam += Fn_PartSep;
    nam += s;
    return *this;
  }
  const char *name() const {
    return nam.c_str();
  }
  bool exist(bool cdir=false) const;
  Filename operator+(const string &s) const {
    return Filename(*this) += s;
  }
  Filename operator+(char const *s) const {
    return Filename(*this) += s;
  }
private:
  string nam;
};

#endif