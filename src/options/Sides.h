// Sides.h

#ifndef SIDES_H

#define SIDES_H

#include <QMap>

namespace Sides {
  enum class Side {
    Solo,
    Left,
    Right
  };
  
  QMap<Side, QString> const &names();
};

#endif
