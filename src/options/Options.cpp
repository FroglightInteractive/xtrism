// Options.cpp

#include "Options.h"
#include <QDir>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "MapIterator.h"
#include "Paths.h"
#include "UserInfo.h"

Options &Options::instance() {
  static Options opts;
  return opts;
}

Options::PPos Options::otherPos(Options::PPos p) {
  if (p==PPos::Left)
    return PPos::Right;
  else
    return PPos::Left;
}

Options::Options() {
  QDir localdata(Paths::datadir());
  QFile fd(localdata.filePath("options.json"));
  if (fd.open(QFile::ReadOnly)) {
    QJsonObject json(QJsonDocument::fromJson(fd.readAll()).object());
    for (auto it: maprange(Sides::names()))
      mk[it.key()]
        = MetaKeys::fromJson(json["metakeys"].toObject()[it.value()].toObject());
    for (auto j: json["players"].toArray()) {
      Player p(Player::fromJson(j.toObject()));
      players[p.id()] = p;
    }
    for (auto j: json["teams"].toArray()) {
      Team t(Team::fromJson(j.toObject()));
      teams[t.ids()] = t;
    }
    { QList<PPos> ppos{PPos::Left, PPos::Right};
      for (auto j: json["curplr"].toArray()) 
        curplr[ppos.takeFirst()] = j.toInt();
    }
    { QList<PPos> ppos{PPos::Left, PPos::Right};
    for (auto j: json["curbs"].toArray()) 
      curbs[ppos.takeFirst()] = j.toInt();
    }
  } else {
    for (auto it: maprange(Sides::names()))
      mk[it.key()] = MetaKeys(it.key());
    Player p;
    p.setName(UserInfo::fullName());
    players[p.id()] = p;
    curplr[PPos::Left] = p.id();
    curplr[PPos::Right] = p.id();
    curbs[PPos::Left] = 0;
    curbs[PPos::Right] = 0;
    save();
  }
}

void Options::save() {
  QJsonObject json;
  QJsonObject jmk;
  for (auto it: maprange(Sides::names()))
    jmk[it.value()] = mk[it.key()].toJson();
  json["metakeys"] = jmk;
  QJsonArray jp;
  for (auto it: maprange(players))
    jp << it.value().toJson();
  json["players"] = jp;
  QJsonArray jt;
  for (auto it: maprange(teams))
    jt << it.value().toJson();
  json["teams"] = jt;
  QJsonArray jcp;
  for (auto it: maprange(curplr))
    jcp << QJsonValue(it.value());
  json["curplr"] = jcp;
  QJsonArray jcb;
  for (auto it: maprange(curbs))
    jcb << QJsonValue(it.value());
  json["curbs"] = jcb;
  
  QDir localdata(Paths::datadir());
  QFile fd(localdata.filePath("options.json"));
  if (fd.open(QFile::WriteOnly)) {
    fd.write(QJsonDocument(json).toJson());
  } else {
    throw "Cannot write options";
  }
}

Options::~Options() {
}

MetaKeys const &Options::metakeys(Sides::Side s) const {
  static MetaKeys dflt;
  return value(mk, s, dflt);
}

QList<int> Options::allPlayerIDs() const {
  return players.keys();
}

Player Options::player(int id) const {
  return players[id];
}

Team Options::team(int id1, int id2) const {
  QPair<int,int> key(id1, id2);
  if (teams.contains(key))
    return teams[key];
  else
    return Team(player(id1), player(id2));
}

int Options::findPlayer(QString name) const {
  for (auto it: maprange(players))
    if (it.value().name()==name)
      return it.key();
  return -1;
}

int Options::currentPlayerID(Options::PPos pos) const {
  return curplr[pos];
}

Player Options::currentPlayer(Options::PPos pos) const {
  return players[curplr[pos]];
}

int Options::currentBrickset(Options::PPos pos) const {
  return curbs[pos];
}

void Options::setCurrentPlayer(Options::PPos pos, int id) {
  curplr[pos] = id;
}

void Options::setCurrentBrickset(Options::PPos pos, int bs) {
  curbs[pos] = bs;
}

void Options::updatePlayer(Player const &p) {
  players[p.id()] = p;
}

void Options::updateTeam(Team const &t) {
  teams[t.ids()] = t;
}

