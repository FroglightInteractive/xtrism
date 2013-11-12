// THandlers.H

#ifndef _THandlers_H
#define _THandlers_H

#include <stack>
#include <vector>

class THKey {
  public:
  virtual ~THKey() {}
    virtual void tkey(int code, bool io)=0;
  };

class THMML {
  public:
  virtual ~THMML() {}
  virtual void tmbc(int /*but*/, bool /*io*/, int /*x*/, int /*y*/) {}
  virtual void tmotion(int /*x*/, int /*y*/) {}
  virtual void tenterleave(bool /*enter_not_leave*/) {}
  };

class THRedraw {
  public:
  virtual ~THRedraw() {}
    virtual void tredraw(int x, int y, int w, int h)=0;
  };

class THQuit {
  public:
  virtual ~THQuit() {}
    virtual void tquit()=0;
  };

class THPoll {
  public:
  virtual ~THPoll() {}
    virtual int tpoll(class TTime const &time)=0;
  };

class THSync {
  public:
  virtual ~THSync() {}
    virtual bool tsync(class TTime const &time)=0;
  };

template <class T> class THStack {
  public:
    bool empty() const { return s.empty(); }
    operator bool() const { return !empty(); }
    T &top() { tthrow(empty(),"THStack: no top"); return *s.top(); }
    void push(T *h) { s.push(h); }
    void pop(T *h) { fthrow(&top()==h,"THStack: illegal pop"); s.pop(); }
  private:
    stack<T*, vector <T*> > s;
  };

#endif
