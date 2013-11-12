// Chooser.C

#include "Chooser.H"
#include <stdio.h>

//////////////////////////////////// CItem ///////////////////////////////////

static string::size_type findword(string const &haystack,
				  string const &needle,
		    string::size_type pos=0) {
  string::size_type k=haystack.find(needle,pos);
  printf("findword %s / %s %i -> %i\n",haystack.c_str(), needle.c_str(),
	 int(pos), int(k));
  if (k==string::npos)
    return k; // at beginning, or not at all
  string b=" "; b+=needle;
  k = haystack.find(b,pos);
  if (k==string::npos)
    return k;
  else
    return k+1;
  }

bool CItem::match(string const &a) const {
  if (a.size()==0)
    return true;
  int i=0;
  string::size_type k=0;
  while (i>=0)
    { int j=a.find(' ',i);
      if (j<0)
        j=a.size();
      string b=a.substr(i,j-i);
      k=findword(text,b,k);
      if (k==string::npos)
        return false;
      i=a.find_first_not_of(' ',j);
    };
  return true;
  }

/////////////////////////////////// Chooser //////////////////////////////////

Chooser &Chooser::operator<<(CItem const &item) {
  list<CItem>::iterator i=items.begin();
  while (i!=items.end() && before(*i,item))
    ++i;
  items.insert(i,item);
  return *this;
  }

void Chooser::resort(bool history) {
  before=CItemBefore(history);
  items.sort(before);
  }

////////////////////////////// Chooser::Iterator /////////////////////////////
Chooser::Iterator::Iterator(string const &pattern0, list<CItem> const &lst):
  pattern(pattern0), end(lst.end()) {
  i=lst.begin();
  if (i==end || (*i).match(pattern))
    return;
  operator++();
  }

Chooser::Iterator &Chooser::Iterator::operator++() {
  if (i!=end)
    while (++i!=end)
      if ((*i).match(pattern))
        break;
  return *this;
  }

