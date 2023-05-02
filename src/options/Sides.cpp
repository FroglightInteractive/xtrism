// Sides.cpp

#include "Sides.h"

namespace Sides {
  QMap<Side, QString> const &names() {
    static QMap<Side, QString> map{
      { Side::Solo, "Solo" },
      { Side::Left, "Left" },
      { Side::Right, "Right" }
    };
    return map;
  }
}

  

  
    
    
