// TDate.C

#include "TDate.H"
#include <time.h>

const int TDLLEN=50;

static char buffer[TDLLEN];

int tdate() {
  return time(0);
  }

char const *tdate(int t, char *d) {
  if (!d)
    d=buffer;
  time_t tt = t;
  strftime(d, TDLLEN, "%d-%m-%Y %H:%M",localtime(&tt));
  return d;
  }
