// timetest.H
// class for doing timing functions &c


#ifndef _timetest_h
#define _timetest_h

#ifndef _timetest_c
typedef long int clock_t;
struct tms {
    clock_t utime;
    clock_t stime;
    clock_t cutime;
    clock_t cstime;
  };
#endif

class TimeTest {
  public:
    TimeTest() { reset(); }
    void print();
    void reset();
  private:
    struct tms t0;
    clock_t c0;
  };

#endif
