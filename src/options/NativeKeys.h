// NativeKeys.h

#ifndef NATIVEKEYS_H

#define NATIVEKEYS_H

#include <QString>

namespace NativeKeys {
  QString name(int scancode);
  int scancode(QString name);
};

#endif
