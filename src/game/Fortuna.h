// Fortuna.H

#ifndef _Fortuna_H
#define _Fortuna_H

#include "../bricks/Probability.h"

class Fortuna {
public:
  Fortuna(Probability const &p);
  ~Fortuna();
  void regother(Fortuna *o);
  void deregother();
  int next();
private:
  Fortuna *other;
  int nextbno;
  int curbno;
  Probability const &prob;
  static bool inited;
};

#endif