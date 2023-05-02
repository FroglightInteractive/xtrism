// Team.cpp

#include "Team.h"
#include "minmax.h"
#include <QJsonArray>

Team::Team(): id1_(0), id2_(0), levels_{50,40,30,20} {
}

Team::Team(Player const &p1, Player const &p2):
  id1_(p1.id()), id2_(p2.id()) {
  for (int k=0; k<4; k++) 
    levels_ << std::min(p1.startLevel(k), p2.startLevel(k));
}

int Team::id1() const {
  return id1_;
}

int Team::id2() const {
  return id2_;
}

QPair<int,int> Team::ids() const {
  return QPair<int,int>(id1_, id2_);
}

int Team::startLevel(int bs) const {
  return levels_[bs];
}

Team Team::fromJson(QJsonObject const &json) {
  Team p;
  p.id1_ = json["id1"].toInt();
  p.id2_ = json["id2"].toInt();
  for (auto val: json["levels"].toArray())
    p.levels_ << val.toInt();
  return p;
}

QJsonObject Team::toJson() const {
  QJsonObject json;
  json["id1"] = id1_;
  json["id2"] = id2_;
  QJsonArray jl;
  for (auto l: levels_)
    jl << l;
  json["levels"] = jl;
  return json;
};
