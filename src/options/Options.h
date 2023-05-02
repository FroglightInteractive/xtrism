// Options.h

#ifndef OPTIONS_H

#define OPTIONS_H

#include "MetaKeys.h"
#include "Player.h"
#include "Team.h"
#include "Sides.h"

class Options {
public:
  enum class PPos {
    Left, Right
  };
public:
  Options();
  ~Options();
public:
  MetaKeys const &metakeys(Sides::Side) const;
  QList<int> allPlayerIDs() const;
  Player player(int id) const;
  Team team(int idleft, int idright) const;
  int findPlayer(QString name) const; // -1 if not found
  Player currentPlayer(PPos) const;
  int currentPlayerID(PPos) const;
  int currentBrickset(PPos) const;
public:
  void setCurrentPlayer(PPos, int);
  void setCurrentBrickset(PPos, int);
  void updatePlayer(Player const &);
  void updateTeam(Team const &);
  void save(); // this is not automatic
private:
  QMap<Sides::Side, MetaKeys> mk;
  QMap<int, Player> players;
  QMap<QPair<int,int>, Team> teams;
  QMap<PPos, int> curplr;
  QMap<PPos, int> curbs;
};

#endif
