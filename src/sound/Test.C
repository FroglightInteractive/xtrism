// Test.C - test for Sound system

// known bug: exceptions don't work v well. :-)

#define NICE

#ifdef NICE
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>

static void alrmhdlr(int a) {
  if (a)
    ;
  }
#endif

#include "SPlayer.H"
#include "Sample.H"
#include <stdio.h>
#include <time.h>
#include <exception>

void my_exc() {
  fprintf(stderr,"Test.C: my_exc (Unexpected exception)\n");
  exit(1);
  }
  
void my_term() {
  fprintf(stderr,
          "Test.C: my_term (eg. exception thrown before exception caught)\n");
  exit(1);
  }

int main() {
  set_terminate(&my_term);
  set_unexpected(&my_exc);
  try {
    Sample s1("/usr/local/lib/xtrism/sound/speedup");
    Sample s2("/usr/local/lib/xtrism/sound/turn");
    
    SPlayer splayer;
    
    char buf[100];
    printf("Press `1' then return for speedup sound, or just return for turn sound\n");
    int t0=time(0);
    bool hoi=0;
#ifdef NICE
    { struct sigaction sa;
      sa.sa_handler=alrmhdlr;
      // sa.sa_mask=0; // Steef's g++ complains about this line...
      sa.sa_flags=SA_RESTART;
      sa.sa_restorer=0;
      sigaction(SIGALRM,&sa,0);
      struct itimerval iv;
      iv.it_interval.tv_sec=0;
      iv.it_interval.tv_usec=1000;
      iv.it_value.tv_sec=0;
      iv.it_value.tv_usec=1000;
      setitimer(ITIMER_REAL,&iv,0); 
    }
#endif
  while (1)
    { int t1=time(0);
      if (t1!=t0)
        { splayer.play(hoi?&s1:&s2,1,.2); printf("play called with %s\n",hoi?"s1":"s2");
          hoi=!hoi;
          t0=t1;
        }
      splayer.poll(); // printf("poll() called\n");
#ifdef NICE
      sigpause(0);
#endif
    }

#ifdef NICE
  { struct itimerval iv;
    iv.it_interval.tv_sec=0; iv.it_interval.tv_usec=1000;
    iv.it_value.tv_sec=iv.it_value.tv_usec=0; setitimer(ITIMER_REAL,&iv,0);
    signal(SIGALRM,SIG_DFL);
  }
#endif
  return 0;
  }
  catch (SampleException x) {
    printf("x=%p\n",&x);
    fprintf(stderr,"Caught an exception\n");
    exit(1);
  }
  }
