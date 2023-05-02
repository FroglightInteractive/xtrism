// MetaKeys.h

#ifndef METAKEYS_H

#define METAKEYS_H

#include <QString>
#include <QJsonObject>
#include "Sides.h"

class MetaKeys {
public:
  enum class Key {
    Pause,
    LevUp,
    LevDown,
    Quit,
  };
  static QMap<Key, QString> const &names();
public:
  MetaKeys(Sides::Side s=Sides::Side::Solo);
  static MetaKeys fromJson(QJsonObject const &);
  QJsonObject toJson() const;
  int operator[](Key k) const { return keys[k]; }
private:
  QMap<Key, int> keys;
};

#endif
