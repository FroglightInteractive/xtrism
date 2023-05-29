// Player.C

#include "Player.h"
#include "MapIterator.h"
#include <QJsonObject>
#include <QJsonArray>

Player::Player(): id_(0),
                  name_("New Player"),
                  levels_{50,40,30,20},
                  nextpos_(1),
                  dwxpos_(true) {
  for (auto it: maprange(Sides::names()))
    keys_[it.key()] = GameKeys(it.key());
}
                  
int Player::id() const {
  return id_;
}

QString Player::name() const {
  return name_;
}

GameKeys Player::keys(Sides::Side s) const {
  return keys_[s];
}

int Player::startLevel(int bs) const {
  return levels_[bs];
}

int Player::nextpos() const {
  return nextpos_;
}

bool Player::dwxpos() const {
  return dwxpos_;
}

Player Player::fromJson(QJsonObject const &json) {
  Player p;
  p.id_ = json["id"].toInt();
  p.name_ = json["name"].toString();
  for (auto sit: maprange(Sides::names()))
    p.keys_[sit.key()]
      = GameKeys::fromJson(json["keys"][sit.value()].toObject());
  p.levels_.clear();
  for (auto val: json["levels"].toArray())
    p.levels_ << val.toInt();
  p.nextpos_ = json["nextpos"].toInt();
  p.dwxpos_ = json["dwxpos"].toBool();
  return p;
}

QJsonObject Player::toJson() const {
  QJsonObject json;
  json["id"] = id_;
  json["name"] = name_;
  QJsonObject jk;
  for (auto it: maprange(Sides::names()))
    jk[it.value()] = keys_.value(it.key()).toJson();
  json["keys"] = jk;
  QJsonArray jl;
  for (auto l: levels_)
    jl << l;
  json["levels"] = jl;
  json["nextpos"] = nextpos_;
  json["dwxpos"] = dwxpos_;
  return json;
};


void Player::setStartLevel(int bs, int lvl) {
  levels_[bs] = lvl;
}

void Player::setKey(Sides::Side s, GameKeys::Key k, int scan) {
  keys_[s].setKey(k, scan);
}
