// PlayerDB.h

#ifndef PLAYERDB_H

#define PLAYERDB_H

#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

enum class PlayerPos {
  Alone,
  Left,
  Right
};
std::map<PlayerPos, std::string> const &playerPosNames();

enum class PlayerKey {
  Left,
  Right,
  RotCW,
  RotCCW,
  Drop,
  Zap
};
std::map<PlayerKey, std::string> &playerKeyNames();
std::list const &playerKeyOrder();

struct Player {
public:
  Player();
  static Player createPlayer();
public:
  int id;
  std::string name;
  std::map<PlayerPos, std::map<PlayerKey, TKeyCode>> keys;
  int startlevel[4];
  int nextpos; // either +1 or -1;
  bool dwxpos;
};
void to_json(json &, Player const &);
void from_json(json const &, Player &);

struct Team {
public:
  Team();
  static Team createTeam(int id1, int id2);
public:
  int idleft;
  int idright;
  int startlevel[4];
};

void to_json(json &, Team const &);
void from_json(json const &, Team &);


class PlayerDB {
public:
  PlayerDB();
  void load(std::istream &is);
  void save(std::ostream &os) const;
  std::list<Player> players;
  std::list<Team> teams;
};

#endif
