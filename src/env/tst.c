#include <sys/time.h>
#include <unistd.h>
#include <signal.h>

struct itimerval iv;
struct timeval tv;

void hdlr(int a) {
  printf("alarm %i!\n",a);
/* printf("setitimer: %i\n",setitimer(ITIMER_REAL,&iv,0)); */
/* printf("%i\n",signal(SIGALRM,hdlr));*/
  }

main() {
  struct sigaction sa;
  sa.sa_handler=hdlr;
  sa.sa_mask=0;
  sa.sa_flags=SA_RESTART;
  sa.sa_restorer=0;
  
  iv.it_interval.tv_sec=0;
  iv.it_interval.tv_usec=1000;
  iv.it_value.tv_sec=0;
  iv.it_value.tv_usec=1000;
sigaction(SIGALRM,&sa,0);
/*  signal(SIGALRM,hdlr);*/
  printf("setitimer %i\n",setitimer(ITIMER_REAL,&iv,0));
  while (1)
    { gettimeofday(&tv,0);
      printf("tv: %i . %i\n",tv.tv_sec,tv.tv_usec);
      /*    printf("getitimer %i\n",getitimer(ITIMER_REAL,&iv));
      printf("--> ival = %i . %i\n",iv.it_interval.tv_sec,
             iv.it_interval.tv_usec);
      printf("--> val = %i . %i\n",iv.it_value.tv_sec,
             iv.it_value.tv_usec);*/
      
      sigpause(0);
/*      setitimer(ITIMER_REAL,&iv,0);*/
    }
  }
