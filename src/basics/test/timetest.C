// timetest.C

#include <sys/times.h>
#define _timetest_c
#include "timetest.H"
#include <stdio.h>

void TimeTest::reset() {
  c0=times(&t0);
  }

void TimeTest::print() {
  clock_t c;
  struct tms t;
  c=times(&t);

  printf("clock  = %5.2f\n",double(c-c0)/CLOCKS_PER_SEC);
  printf("user   = %5.2f\n",double(t.tms_utime-t0.tms_utime)/CLOCKS_PER_SEC);
  printf("system = %5.2f\n",double(t.tms_stime-t0.tms_stime)/CLOCKS_PER_SEC);
  printf("childU = %5.2f\n",
         double(t.tms_cutime-t0.tms_cutime)/CLOCKS_PER_SEC);
  printf("childS = %5.2f\n",
         double(t.tms_cstime-t0.tms_cstime)/CLOCKS_PER_SEC);
  }
