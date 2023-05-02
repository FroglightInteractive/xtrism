// Sample.h

#ifndef SAMPLE_H

#define SAMPLE_H

#include <QString>

class Sample {
public:
  Sample(QString fn);
  void play();
private:
  QString cmd;
};

#endif
