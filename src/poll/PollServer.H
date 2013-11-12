// PollServer.H

#ifndef _PollServer_H
#define _PollServer_H

#include <list>
#include "../env/TTime.H"

class PollServer {
  public:
    PollServer();
    ~PollServer();
    int poll(class TTime const &now);
  public: // for Sleeper
    void request(class Sleeper *slpr, class TTime const &when);
    void forget(class Sleeper *slpr);
  private:
  std::list<std::pair<class TTime, class Sleeper *> > events;
    TTime first;
  };

#endif
