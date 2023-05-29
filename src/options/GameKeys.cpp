// GameKeys.cpp

#include "GameKeys.h"
#include "MapIterator.h"
#include "NativeKeys.h"

QMap<GameKeys::Key, QString> const &GameKeys::names() {
  static QMap<Key, QString> map{
    { Key::Left, "Left" },
    { Key::Right, "Right" },
    { Key::CW, "Rotate CW" },
    { Key::CCW, "Rotate CCW" },
    { Key::Drop, "Drop" },
    { Key::Zap, "Zap" },
    { Key::FloatDown, "Float Down" } };
  return map;
}

GameKeys::GameKeys(Sides::Side s) {
  static QMap<Sides::Side, QMap<Key, QString>> dflt{
    { Sides::Side::Solo, {
      { Key::Left, "Comma" },
      { Key::Right, "Period" },
      { Key::CW, "L. Alt" },
      { Key::CCW, "A" },
      { Key::Drop, "CapsLock" },
      { Key::Zap, "R. Shift" },
      { Key::FloatDown, "L. Shift" } } },
    { Sides::Side::Left, {
      { Key::Left, "S" },
      { Key::Right, "D" },
      { Key::CW, "L. Alt" },
      { Key::CCW, "A" },
      { Key::Drop, "CapsLock" },
      { Key::Zap, "F" } } },
    { Sides::Side::Right, {
      { Key::Left, "KP 4" },
      { Key::Right, "KP 6" },
      { Key::CW, "KP 8" },
      { Key::CCW, "KP 5" },
      { Key::Drop, "KP Enter" },
      { Key::Zap, "R. Shift" } } }
  };
  for (auto it: maprange(dflt[s])) 
    keys[it.key()] = NativeKeys::scancode(it.value());
}

GameKeys GameKeys::fromJson(QJsonObject const &json) {
  GameKeys mk;
  for (auto kit: maprange(GameKeys::names()))
    if (json.contains(kit.value()))
      mk.keys[kit.key()] = json[kit.value()].toInt();
  return mk;
}

QJsonObject GameKeys::toJson() const {
  QJsonObject json;
  for (auto kit: maprange(GameKeys::names()))
    json[kit.value()] = keys[kit.key()];
  return json;
}
  
QList<int> GameKeys::allCodes() const {
  QList<int> res;
  for (auto kit: maprange(keys))
    res << kit.value();
  return res;
}

void GameKeys::setKey(GameKeys::Key k, int scan) {
  keys[k] = scan;
}
