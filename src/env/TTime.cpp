// TTime.C

#include "TTime.h"

#include <sys/time.h>
#include <unistd.h>
#include "../basics/Infty.h"
#include "../basics/Throw.h"

static TTime startuppseudotime(TTime::MAGIC_DONTUSE);

int TTime::cache = 0;

TTime &TTime::operator=(TTime::Setup const &x) {
  switch (x) {
  case MAGIC_DONTUSE: {
    struct timeval tv;
    tthrow(gettimeofday(&tv, 0), "TTime: can't read time");
    time = tv.tv_sec;
  } break;

  case CURRENT: {
    struct timeval tv;
    tthrow(gettimeofday(&tv, 0), "TTime: can't read time");
    time = 1000 * (tv.tv_sec - startuppseudotime.time) + tv.tv_usec / 1000;
    cache = time;
  } break;

  case CACHED: {
    time = cache;
    break;
  }

  case INFINITE_FUTURE:
    time = INFTY;
    break;

  case INFINITE_PAST:
    time = -INFTY;
    break;
  }
  return *this;
}