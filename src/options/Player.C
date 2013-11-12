// Player.C

#include <iostream>
#include <string>
#include "Player.H"
#include "../basics/getline.H"
#include "../basics/vecio.H"
#include <backward/strstream>
#include "../basics/dbx.H"

const int PL_LLEN=1000;

//static char pl_linebuf[PL_LLEN];

istream &operator>>(istream &is, Player &pl) {
  string s(getline_nocmt(is));
  istrstream iss(s.c_str());
  iss >> pl.id_; skipwhite(iss);
  string q; bool first=1;
  while (!iss.eof())
    { iss >> q; if (!first) pl.name_+=' ';
      pl.name_+=q; first=0; }
  // getline((istream&)iss,pl.name_); //=pl_linebuf;
  // dbx(-990000,"istream >> Player: %i `%s'",pl.id_, pl.name_.c_str());

  pl.level_.erase(pl.level_.begin(),pl.level_.end());

  getline_nocmt(is) >> pl.level_; 

  s=getline_nocmt(is);
  pl.next_left=s[0]=='<';
  pl.dw_xpos=s[1]=='D';

  pl.keys_alone.erase(pl.keys_alone.begin(),pl.keys_alone.end());
  pl.keys_left.erase(pl.keys_left.begin(),pl.keys_left.end());
  pl.keys_right.erase(pl.keys_right.begin(),pl.keys_right.end());
  
  getline_nocmt(is) >> pl.keys_alone;
  getline_nocmt(is) >> pl.keys_left;
  getline_nocmt(is) >> pl.keys_right;

  return is;
  }  

ostream &operator<<(ostream &os, Player const &pl) {
  os << pl.id_ << ' ' << pl.name_ << endl;
  os << pl.level_ << endl;
  os << (pl.next_left?'<':'>') << endl;
  os << pl.keys_alone << endl;
  os << pl.keys_left << endl;
  os << pl.keys_right << endl;

  return os;
  }

vector<TKeyCode> const &Player::keys(int where /* in {-1,0,1} */) const {
  if (where==0)
    return keys_alone;
  else if (where<0)
    return keys_left;
  else
    return keys_right;
  }

short Player::level(unsigned int idx) const {
  if (idx<level_.size())
    return level_[idx];
  else
    return 0;
  }

Player &Player::operator=(Player const &p) {
  id_=p.id_;
  name_=p.name_;
  keys_alone=p.keys_alone;
  keys_left=p.keys_left;
  keys_right=p.keys_right;
  next_left=p.next_left;
  level_=p.level_;
  return *this;
  }
