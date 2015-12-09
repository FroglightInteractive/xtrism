// Sample.C

// Naming convention for sample files:
// name-freq.sw

#include "Sample.H"

#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

Sample::Sample(char const *name) throw (SampleExc) {
  // note: name does contain directory stuff, but not the -freq.sw part.
  data=0;
  char buf[1000];
  int i;
  if ((i=readlink(name,buf,1000))<0) {
    strncpy(buf, name, 999);
    // perror("Sample: readlink");
    // perror(name);
    // throw SampleExc();
  } else {
    buf[i]='\0';
  }
  
  char *minus=strrchr(buf,'-');
  if (!minus)
    { fprintf(stderr,"Sample: bad filename"); throw SampleExc(); }
  sfreq_=atoi(minus+1);

  char buf2[1000];
  if (buf[0]=='/')
    strcpy(buf2,buf);
  else
    { char const *slash=strrchr(name,'/');
      memcpy(buf2,name,slash+1-name);
      strcpy(buf2+(slash+1-name),buf);
    }
  struct stat stb;
//  printf("%s\n",buf2);
  if (stat(buf2,&stb))
    { perror("Sample: stat"); throw SampleExc(); }
  length_=stb.st_size/sizeof(SampleVal);

  data=new SampleVal[length_*sizeof(SampleVal)];

  FILE *f=fopen(buf2,"rb");
  if (!f)
    { perror("Sample: fopen"); throw SampleExc(); }
  if (fread(data,sizeof(SampleVal),length_,f)<length_)
    { perror("Sample: fread"); throw SampleExc(); }
  fclose(f);
  }
