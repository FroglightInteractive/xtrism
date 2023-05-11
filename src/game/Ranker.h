// Ranker.H

#ifndef _Ranker_H
#define _Ranker_H

#include <QString>
#include "Records.h"

class Ranker {
public:
  Ranker(QString name, int bs);
  QString getRank(int score);
private:
  int currentidx;
  QList<int> scores;
};

#endif
