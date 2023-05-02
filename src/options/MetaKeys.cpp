// MetaKeys.cpp

#include "MetaKeys.h"
#include "MapIterator.h"
#include "NativeKeys.h"

QMap<MetaKeys::Key, QString> const &MetaKeys::names() {
  static QMap<Key, QString> map{
    { Key::Pause, "Pause" },
    { Key::LevUp, "Level Up" },
    { Key::LevDown, "Level Down" },
    { Key::Quit, "Quit" }};
  return map;
}

MetaKeys::MetaKeys(Sides::Side s) {
  static QMap<Sides::Side, QMap<Key, QString>> dflt{
    { Sides::Side::Solo,
      { { Key::Pause, "1" },
        { Key::LevUp, "2" },
        { Key::LevDown, "3" },
        { Key::Quit, "Esc" } } },
    { Sides::Side::Left,
      { { Key::Pause, "1" },
        { Key::LevUp, "2" },
        { Key::LevDown, "3" },
        { Key::Quit, "Esc" } } },
    { Sides::Side::Right,
      { { Key::Pause, "Backspace" },
        { Key::LevUp, "=" },
        { Key::LevDown, "-" },
        { Key::Quit, "0" } } },
  };
  for (auto it: maprange(dflt[s]))
    keys[it.key()] = NativeKeys::scancode(it.value());
}

MetaKeys MetaKeys::fromJson(QJsonObject const &json) {
  MetaKeys mk;
  for (auto kit: maprange(MetaKeys::names()))
    mk.keys[kit.key()] = json[kit.value()].toInt();
  return mk;
}

QJsonObject MetaKeys::toJson() const {
  QJsonObject json;
  for (auto kit: maprange(MetaKeys::names()))
    json[kit.value()] = keys[kit.key()];
  return json;
}
  
