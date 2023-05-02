// Player.H

#ifndef _Player_H
#define _Player_H

#include <QJsonObject>
#include "GameKeys.h"

class Player {
public:
  Player();
  int id() const;
  QString name() const;
  GameKeys keys(Sides::Side) const;
  int startLevel(int bs) const;
  int nextpos() const;
  bool dwxpos() const;
public:
  static Player fromJson(QJsonObject const &);
  QJsonObject toJson() const;
private:
  int id_;
  QString name_;
  QMap<Sides::Side, GameKeys> keys_;
  QList<int> levels_;
  int nextpos_;
  bool dwxpos_;
};

#endif
