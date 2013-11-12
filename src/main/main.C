// main.C

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "../env/TEnv.H"
#include "../globals/Globals.H"
#include "../mainmenu/MainMenu.H"
#include <exception>
#include "../.datadir"
#include "../basics/Filename.H"

void my_exc() {
  fprintf(stderr,"XTrism: my_exc (Unexpected exception)\n");
  exit(1);
  }
  
void my_term() {
  fprintf(stderr,
          "XTrism: my_term (eg. exception thrown before exception caught)\n");
  exit(1);
  }

#include "../gfx/Button.H"
MainMenu *mmp;
void setlastscore(int sc, int li, double ppb, char const *name, int bs) {
  char buf[300];
  sprintf(buf,"%s: %i in %i lines (%.1f ppb); bset %i - ",name,sc,li,ppb,bs+1);
  time_t t=time(0);
  strftime(buf+strlen(buf),100,"%d %b %Y %H:%M",localtime(&t));
  if (sc!=0)
    { string s=DATADIR; s+="/hisc";
      FILE *f=fopen(s.c_str(),"a");
      if (f)
        { fprintf(f,"%s\n",buf); fclose(f); }
    }
  mmp->textbut->settext(buf);
  }

int main(int argc, char **argv) {
  int r=0;
  set_terminate(&my_term);
  set_unexpected(&my_exc);
  dbx(1,"start of main");
/*  char *b=new char[1000*1000];
  char *c=new char[1000*1000];
  char *d=new char[1000*1000];
  for (int i=0; i<1000*1000; i+=581)
    { b[i]=123; c[i]=63; d[i]=78; } */
  
  try
    {
      global_init(argc, argv);
//      throw "Hello world";
      
      mmp=new MainMenu;
      tenv().loop();
      delete mmp;
      global_destroy();
    }
  catch (string &s)
    { printf("exc:main.string: %s\n",s.c_str()); }
  catch (char *s)
    { printf("exc:main.char*: %s\n",s); }
  catch (...)
    { printf("exc:main....: unknown expection\n"); }
  dbx(1,"end of main");
  return r;
  }
