// Sleeper.H

#ifndef _Sleeper_H
#define _Sleeper_H

#include <list>

class Sleeper {
  public:
    Sleeper(class PollServer &server);
    virtual ~Sleeper();
    virtual void poll() {}
  public: // for Waker
    virtual void warn(class Waker const *waker=0, int code=0);
    /* warn() is virtual so descendents may make note of who is
       waking them and why. */
    void waker_dying(class Waker const *waker);
  public: // for PollServer
    void s_poll() { warned=false; poll(); }
  protected: // for children
    void sendreq(class TTime const &when);
    void sendreq(int dt);
    void regwaker(class Waker *waker);
  private:
    class PollServer &server;
  std::list<class Waker *> wakers;
    bool warned;
  };

#endif
