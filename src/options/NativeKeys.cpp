// NativeKeys.cpp

#include "NativeKeys.h"
#include "MapIterator.h"

#include <QMap>

namespace NativeKeys {
#include "nativescancodes.h"

  QString name(int scancode) {
    return scan2name[scancode];
  }
  
  int scancode(QString name) {
    static QMap<QString, int> revmap;
    if (revmap.isEmpty()) 
      for (auto it: maprange(scan2name))
        revmap[it.value()] = it.key();
    if (revmap.contains(name))
      return revmap[name];
    else
      return -1;
  }
};
