// Probability.H

// The probabilities for each brick

#ifndef _Probability_H
#define _Probability_H

#include <QList>

class Probability {
public:
  Probability(QMap<int, float> probs);
  int select(float x) const;
private:
  QList<float> thrs;
  QList<int> res;
};

Probability const &probabilities(int bset);

#endif
