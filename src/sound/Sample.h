// Sample.h

#ifndef SAMPLE_H

#define SAMPLE_H

#include <QString>

class Sample {
public:
  Sample(char const *fn);
  void play();
private:
  QString cmd;
};

#endif
