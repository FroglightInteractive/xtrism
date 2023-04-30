// PlayerDB.cpp

#include "PlayerDB.h"

std::map<PlayerPos, std::string> const &playerPosNames() {
  static std::map<PlayerPos, std::string> pp{
    { PlayerPos::Alone, "alone" },
    { PlayerPos::Left, "left" },
    { PlayerPos::Right, "right" }
  };
  return pp;
}

std::map<PlayerKey, std::string> const &playerKeyNames() {
  static std::map<PlayerKey, std::string> kk{
    { PlayerKey::Left, "left" },
    { PlayerKey::Right, "right" },
    { PlayerKey::RotCW, "cw" },
    { PlayerKey::RotCCW, "ccw" },
    { PlayerKey::Drop, "drop" },
    { PlayerKey::Zap, "zap" }
  }
  return kk;
}

std::list const &playerKeyOrder() {
  static std::list kk{
    PlayerKey::Left,
    PlayerKey::Right,
    PlayerKey::RotCW,
    PlayerKey::RotCCW,
    PlayerKey::Drop,
    PlayerKey::Zap,
  };
  return kk;
}

Player::Player() {
  id = 0;
}

Player Player::createPlayer() {
  Player p;
  static json j = {
    { "id", 0 },
    { "name", "" },
    { "nextpos", 1 },
    { "dwxpos", true },
    { "startlevel", { 125, 105, 85, 70 }},
    {"keys", {
        { "alone", {
            { "left", 59 },
            { "right", 60 },
            { "cw", 64 },
            { "ccw", 38 },
            { "drop", 66 },
            { "zap", 62 },
          }
        },
        { "left", {
            { "left", 39 },
            { "right", 60 },
            { "cw", 64 },
            { "ccw", 38 },
            { "drop", 66 },
            { "zap", 41 },
          }
        },
        { "right", {
            { "left", 44 },
            { "right", 46 },
            { "cw", 31 },
            { "ccw", 47 },
            { "drop", 45 },
            { "zap", 65 }
          }
        }
      }
    }
  };
  from_json(j, p);
  return p;
}

void to_json(json &j, Player const &p) {
  j = json{{"id", p.id},
           {"name", p.name},
           {"startlevel", p.startlevel},
           {"nextpos", p.nextpos},
           {"dwxpos", p.dwxpos}};
  for (auto const &pp: playerPosNames()) 
    for (auto const &kk: playerKeyNames()) 
      j["keys"][pp.second][kk.second] = p.keys[pp.first][kk.first];
}
  
void from_json(json const &j, Player &p) {
  j.at("id").get_to(p.id);
  j.at("name").get_to(p.name);
  j.at("startlevel").get_to(p.startlevel);
  j.at("nextpos").get_to(p.nextpos);
  for (auto const &pp: playerPosNames()) 
    for (auto const &kk: playerKeyNames()) 
      j.at("keys").at(pp.second).at(kk.second)
        .get_to(p.keys[pp.first][kk.first]);
}

Team::Team() {
}

Team Team::createTeam(int id1, int id2) {
  Team t;
  t.idleft = id1;
  t.idright = id2;
  startlevel[0] = 25;
  startlevel[1] = 20;
  startlevel[2] = 10;
  startlevel[3] = 5;
  return t;
}

void to_json(json &j, Team const &t) {
  j = json{{"idleft", t.idleft},
           {"idright", t.idright},
           {"startlevel", t.startlevel}};
}

void from_json(json const &j, Team &t) {
  j.at("idleft").get_to(p.idleft);
  j.at("idright").get_to(t.idright);
  j.at("startlevel").get_to(t.startlevel);
}

PlayerDB::PlayerDB() {
}

void PlayerDB::load(std::istream &is) {
  players.clear();
  teams.clear();
  json j = json::parse(is);
  if (j.contains("players")) {
    for (auto const &el: j["players"]) {
      Player p;
      from_json(el, p);
    }
  }
