// Fortuna.C

/* Fortuna uses rand(). It calls srand() exactly once, the first time
   a Fortuna is constructed. It uses the current time (through TTime)
   to obtain a seed.
 * Make sure that the probability distribution can return at least 4
   different values with non-infinitesimal probability. Otherwise next()
   may loop forever.
 */

#include "Fortuna.h"
#include <stdlib.h>
#include "../env/TTime.h"

bool Fortuna::inited = false;

Fortuna::Fortuna(Probability const &p):
  other(0), nextbno(0), curbno(0), prob(p) {
  if (!inited)
    srand(TTime(TTime::CURRENT).intrep());
  inited = true;
}

Fortuna::~Fortuna() {
  if (other)
    other->deregother();
}

void Fortuna::regother(Fortuna *o) {
  other = o;
  other->other = this;
}

void Fortuna::deregother() {
  if (other)
    other->other = 0;
  other = 0;
}

int Fortuna::next() {
  curbno = nextbno;
  do {
    nextbno = prob(float(rand()) / RAND_MAX);
  } while (nextbno == curbno
           || (other
               && (nextbno == other->curbno || nextbno == other->nextbno)));
  return nextbno;
}