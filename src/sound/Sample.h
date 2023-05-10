// Sample.h

#ifndef SAMPLE_H

#define SAMPLE_H

#include <QString>
#include <QVector>

class Sample: public QVector<short> {
public:
  Sample(QString fn);
};

#endif
