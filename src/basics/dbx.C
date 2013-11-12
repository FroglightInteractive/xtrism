/* dbx.C */

#include <stdio.h>
#include <stdarg.h>

#include "dbx.H"

#define DBXDATE -980824
#define DBXLEVEL 1

void dbx(int lvl, char *fmt, ...) {
  va_list ap;
  if (lvl>0) {
    if (lvl>DBXLEVEL)
      return;
  } else {
    if (lvl>DBXDATE)
      return;
  }
  va_start(ap,fmt);
  fprintf(stderr,"[dbx/%i: ",lvl);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr,"]\n");
  }
