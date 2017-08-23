// Waker.C

#include "Waker.H"
#include "Sleeper.H"
#include "../basics/Throw.H"

#include <algorithm>

#include "../basics/dbx.H"

Waker::~Waker() {
  dbx(-980809, "Waker::~Waker");
  for (list<Sleeper *>::iterator i = sleepers.begin();
       i != sleepers.end();
       ++i) {
    dbx(-980809, "  sending waker_dying to %p", *i);
    (*i)->waker_dying(this);
  }
  dbx(-980809, "  done");
}

void Waker::reg(Sleeper *sleeper) {
  dbx(-980809, "Waker (%p)::reg %p", this, sleeper);

  if (find(sleepers.begin(), sleepers.end(), sleeper) != sleepers.end())
    athrow("Waker::reg(): sleeper already registered");
  sleepers.push_back(sleeper);
}

void Waker::dereg(Sleeper const *sleeper) {
  dbx(-980809, "Waker (%p)::dereg %p", this, sleeper);
  for (list<Sleeper *>::iterator i = sleepers.begin();
       i != sleepers.end();
       ++i)
    if ((*i) == sleeper) {
      sleepers.erase(i);
      return;
    }
  athrow("Waker::dereg(): unknown sleeper");
}

void Waker::sendwarn(int code) {
  for (list<Sleeper *>::iterator i = sleepers.begin();
       i != sleepers.end();
       ++i)
    (*i)->warn(this, code);
}