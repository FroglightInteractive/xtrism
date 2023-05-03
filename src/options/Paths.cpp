// Paths.cpp

#include "Paths.h"
#include <QStandardPaths>

namespace Paths {

  QString datadir() {
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  }

  QString cachedir() {
    return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
  }

}
