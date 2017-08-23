// GlobalOpts.C

#include "GlobalOpts.H"
#include "../basics/getline.H"
#include "../basics/vecio.H"
#include <backward/strstream>

using namespace std;

#include "../basics/dbx.H"

istream &operator>>(istream &is, GlobalOpts &go) {
  dbx(2, "istream >> GlobalOpts");
  getline_nocmt(is) >> go.keys_alone;
  getline_nocmt(is) >> go.keys_left;
  getline_nocmt(is) >> go.keys_right;
  dbx(3, "istream >> GlobalOpts done");
  return is;
}

ostream &operator<<(ostream &os, GlobalOpts const &go) {
  os << go.keys_alone << endl;
  os << go.keys_left << endl;
  os << go.keys_right << endl;
  return os;
}