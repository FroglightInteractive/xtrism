// Team.h

#ifndef TEAM_H

#define TEAM_H

#include "Player.h"
#include <QJsonObject>
#include <QPair>

class Team {
public:
  Team();
  Team(Player const &p1, Player const &p2);
  int id1() const;
  int id2() const;
  QPair<int,int> ids() const;
  int startLevel(int bs) const;
  void setStartLevel(int bs, int lvl);
public:
  static Team fromJson(QJsonObject const &);
  QJsonObject toJson() const;
private:
  int id1_, id2_;
  QList<int> levels_;
};

#endif
