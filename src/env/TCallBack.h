// TCallBack.H

#ifndef _TCallBack_H
#define _TCallBack_H

#include <list>
#include "../basics/Throw.h"

class TCallBack {
public:
  virtual ~TCallBack() {
  }
  virtual void callback(int code=0) = 0;
};

class TCBList {
private:
  class TCBElt {
public:
    TCBElt(TCallBack *tcb0, int code0=0): tcb(tcb0), code(code0) {
    }
    TCBElt() {
    }
    bool operator==(TCBElt const &oth) const {
      return tcb == oth.tcb;
    }
private:
    friend class TCBList;
    TCallBack *tcb;
    int code;
  };
public:
  bool empty() const {
    return s.empty();
  }
  operator bool() const {
    return !empty();
  }
  void callpop() {
    tthrow(empty(), "TCBList: empty");
    TCBElt tc = *s.begin();
    s.erase(s.begin());
    tc.tcb->callback(tc.code);
  }
  void callpopall() {
    while (!empty()) callpop();
  }
  void reg(TCallBack *h, int c=0) {
    s.push_back(TCBElt(h, c));
  }
  void dereg(TCallBack *h) {
    s.remove(TCBElt(h));
    /* no fault if h already popped: dereg() while
       threaded from callpop() is OK.
     */}
private:
  list<TCBElt> s;
};

#endif