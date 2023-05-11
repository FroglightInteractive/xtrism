// Ranker.C

#include "Ranker.h"
#include "Records.h"
#include "Sounds.h"

Ranker::Ranker(QString name, int bs) {
  AllRecords rrr(AllRecords::instance());
  Records const &rr(rrr[name][bs]);
  for (Record const &r: rr)
    scores << r.score;
  currentidx = scores.size();
}

QString Ranker::getRank(int score) {
  while (currentidx>0 && score>scores[currentidx-1]) {
    if (currentidx==Records::MAXKEEP)
      Sounds::instance()->applause();
    currentidx --;
    if (currentidx==0)
      Sounds::instance()->applause();
  }
  if (currentidx==Records::MAXKEEP)
    return "-";
  switch (currentidx) {
  case 0: return "1st";
  case 1: return "2nd";
  case 2: return "3rd";
  default: return QString("%1th").arg(currentidx+1);
  }
}  
