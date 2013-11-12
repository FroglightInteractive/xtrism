// Sleeper.C

#include "Sleeper.H"
#include "Waker.H"
#include "PollServer.H"
#include "../basics/Throw.H"

#include <algo.h>

#include "../basics/dbx.H"

Sleeper::Sleeper(PollServer &serv0): server(serv0), warned(false) {
  }

Sleeper::~Sleeper() {
  dbx(-980809,"~Sleeper %p",this);
  for (list<Waker *>::iterator i=wakers.begin(); i!=wakers.end(); ++i)
    { dbx(-980809,"  dereg'ing %p",*i);
      (*i)->dereg(this);
    }
  dbx(-980809,"  server.forget");
  server.forget(this);
  }

void Sleeper::warn(Waker const *waker, int code) {
  if (warned)
    return;
  warned=true;
  sendreq(0);
  }

void Sleeper::waker_dying(Waker const *waker) {
  dbx(-980809,"Sleeper (%p)::waker_dying %p",this,waker);
  
  for (list<Waker *>::iterator i=wakers.begin(); i!=wakers.end(); ++i)
    if ((*i)==waker)
      { wakers.erase(i);
        dbx(-980809,"  done");
        return;
      }
  athrow("Sleeper::waker_dying(): unknown waker");
  }

void Sleeper::sendreq(TTime const &when) {
  server.request(this,when);
  }

void Sleeper::sendreq(int dt) {
  server.request(this,TTime(TTime::CACHED) + dt);
  }

void Sleeper::regwaker(Waker *waker) {
  dbx(-980809,"Sleeper (%p)::regwaker %p",this,waker);
  if (find(wakers.begin(),wakers.end(),waker)!=wakers.end())
    athrow("Sleeper::regwaker(): waker already registered");
  wakers.push_back(waker); waker->reg(this);
  dbx(-980809,"  done");
  }
