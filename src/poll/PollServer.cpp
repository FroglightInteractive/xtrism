// PollServer.C

#include "PollServer.h"

#include "../env/TTime.h"
#include "Sleeper.h"
#include "../basics/Infty.h"
#include "../basics/dbx.h"

PollServer::PollServer(): first(TTime::INFINITE_FUTURE) {
}

PollServer::~PollServer() {
  dbx(-980809, "PollServer (%p)::~PollServer", this);
}

int PollServer::poll(TTime const &now) {
  if (first > now)
    return first - now;

  std::list<std::pair<TTime, Sleeper *> >::iterator i, j, last;
  last = events.end();
  bool acted = false; // DEBUG!
  for (i = events.begin(); i != last; ) {
    j = i;
    i++;
    if ((*j).first <= now) {
      dbx(-980809, "(%p @ %i)  polling %p", this, now.intrep(), (*j).second);
      acted = true;
      Sleeper *sl = (*j).second;
      events.erase(j);
      sl->s_poll();
      dbx(-980809, "(%p) returned from %p", this, (*j).second);
    }
  }
  first = TTime::INFINITE_FUTURE;
  for (i = events.begin(); i != events.end(); ++i)
    if ((*i).first < first)
      first = (*i).first;
  if (acted) dbx(-980809, "  returning %i", first - now);
  return first - now;
}

void PollServer::request(Sleeper *slpr, TTime const &when) {
  if (when < first)
    first = when;
  events.push_back(std::pair<TTime, Sleeper *>(when, slpr));
}

void PollServer::forget(Sleeper *slpr) {
  std::list<std::pair<TTime, Sleeper *> >::iterator i, j;
  for (i = events.begin(); i != events.end(); ) {
    j = i;
    ++i;
    if ((*j).second == slpr)
      events.erase(j);
  }
}