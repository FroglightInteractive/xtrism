#include <stdio.h>

#include <unistd.h>
#include <sys/time.h>

void ptime() {
  struct timeval tv;
  struct timezone tz;
  gettimeofday(&tv,&tz);
  printf("%i %i\n",tv.tv_sec, tv.tv_usec);
  }

int main(void) {
  for (int i=0; i<10; i++)
    { ptime();
      usleep(10);
    }
  ptime();
  wait
  sleep(100);
  ptime();
  return 0;
}
