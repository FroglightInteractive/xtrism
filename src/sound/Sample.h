// Sample.h

#ifndef SAMPLE_H

#define SAMPLE_H

#include <QString>
#include <QVector>

class Sample: public QVector<short> {
public:
  Sample(QString fn);
  QString name() const { return fn; }
private:
  QString fn;
};

#endif
