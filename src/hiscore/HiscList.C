// HiscList.C

#include "HiscList.H"
#include "../env/TCompress.H"
#include "../basics/Throw.H"

HiscList::HiscList(int profile, int ath):
  profile_(profile), ath_(ath) {
  if (ath)
    athl=new ATHList_;
  else
    hl=new HiscList_;
  }

HiscList::HiscList(TDecompress &src) {
  char buf[HLLEN];
  fthrow(src.readln(buf),"HiscList: can't read from TDecompress");
  tthrow(sscanf(buf,"%i %i",&profile_,&ath_)<2,\
         "HiscList: can't read profile or ath flag");
  if (ath)
    athl=new ATHList_;
  else
    hl=new HiscList_;
 
  while (src.readln(buf))
    { if (ath)
        insert(ATHLine(buf));
      else
        insert(HiscLine(buf));
    }
  }

void HiscList::insert(ATHLine const &athl) {
  adfbjadk;lfgsadf
             ;
  }

void HiscList::insert(HiscLine const &hl) {
  asdghkaljdfgsdfg;
  }
