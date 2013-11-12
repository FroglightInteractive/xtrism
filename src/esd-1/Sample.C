// Sample.C (ESD vsn)

#include "Sample.H"
#include "../basics/dbx.H"

#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include <esd.h>

int Sample::esd_handle = 0;

Sample::Sample(char const *fn) throw (SampleExc):
  name_(fn), id_(-1) { 
  if (!esd_handle) {
    fprintf(stderr, "Sample: no esd");
    throw SampleExc(); // can't work w/o esd connection
  }

  char buf[1000];
  int i;
  if ((i=readlink(fn,buf,1000))<0)
    { perror("Sample: readlink"); fprintf(stderr,"\n%s\n",fn); throw SampleExc(); }
  buf[i]='\0';
  
  char *minus=strrchr(buf,'-');
  if (!minus)
    { fprintf(stderr,"Sample: bad filename"); throw SampleExc(); }
  sfreq_=atoi(minus+1);

  struct stat stb;
  if (stat(fn,&stb))
    { perror("Sample: stat"); throw SampleExc(); }
  length_=stb.st_size/sizeof(SampleVal);

  // cache the sample
  dbx(-990925,"Caching sample '%s'",name_.c_str());
  id_ = esd_sample_cache(esd_handle, ESD_BITS16 | ESD_MONO | ESD_SAMPLE,
			 sfreq_, length_*sizeof(SampleVal),
			 name_.c_str());
  dbx(-990925,"Cached: %i",id_);
  if (id_<0) {
    perror("Sample: cache"); throw SampleExc(); 
  }
}

Sample::~Sample() {
  if (id_>=0)
    esd_sample_free(esd_handle,id_);
}

