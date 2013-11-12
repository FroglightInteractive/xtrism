// HiscLine.C

#include "HiscLine.H"
#include <string.h>
#include "../env/TDate.H"

HiscLine::HiscLine(char const *s) {
  sscanf(s,"%i %hu %u %hu %hu",
         &date, &slev, &score, &lines, &blocks);
  }

HiscLine::HiscLine(float score0, int lines0, int blocks0, int slev0) {
  score=int(score0);
  lines=(unsigned short)(lines0);
  blocks=(unsigned short)(blocks0);
  slev=(unsigned short)(slev0);
  date=tdate();
  }

char const *HiscLine::read(char *s) const {
  if (!s)
    s=buffer;
  sprintf(s,"%i %hu %u %hu %hu",
          date,slev,score,lines,blocks);
  return s;
  }

ATHLine::ATHLine(char const *s):
  HiscLine(s) {
  char *lastsp=strrchr(s,' ');
  sscanf(lastsp,"%i",&plrid);
  }

ATHLine::ATHLine(int plrid0, float score, int lines, int blocks, int slev):
  HiscLine(score, lines, blocks, slev), plrid(plrid0) {
  }

char const *ATHLine::read(char *s) {
  HiscLine::read(s);
  s=s?(s+strlen(s)):(buffer+strlen(buffer));
  sprintf(s," %i",plrid);
  return s;
  }
