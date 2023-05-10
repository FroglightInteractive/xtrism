// Sample.cpp

#include "Sample.h"
#include <QFile>
#include <cstring>
#include <QFileInfo>
#include <QDebug>

Sample::Sample(QString fn) {
  QFileInfo fi(fn);
  Q_ASSERT_X(fi.exists(), "Sample", "Failure to open sound file");
  int size = fi.size();
  resize(size / 2);
  qDebug() << "sample" << fn << fi.exists() << size;

  QFile fd(fn);
  fd.open(QFile::ReadOnly);
  fd.read((char*)(data()), size);
}
  
