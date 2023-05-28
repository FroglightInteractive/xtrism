// GameKeys.h

#ifndef GAMEKEYS_H

#define GAMEKEYS_H

#include <QString>
#include "Sides.h"
#include <QJsonObject>

class GameKeys {
public:
  enum class Key {
    Left,
    Right,
    CW,
    CCW,
    Drop,
    Zap,
    FloatDown,
  };
  static QMap<Key, QString> const &names();
public:
  GameKeys(Sides::Side s=Sides::Side::Solo);
  static GameKeys fromJson(QJsonObject const &);
  QJsonObject toJson() const;
  int operator[](Key k) const { return keys[k]; }
  QList<int> allCodes() const;
private:
  QMap<Key, int> keys;
};

#endif
