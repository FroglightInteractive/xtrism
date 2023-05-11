// Records.cpp

#include "Records.h"
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QVariantList>
#include <QDir>
#include "Paths.h"

Record::Record(int score, int lines, int bricks, QString date1):
  date(date1), score(score), lines(lines), bricks(bricks) {
  if (date=="")
    date = QDateTime::currentDateTime().toString("yyyyMMdd.hhmmss");
}

bool Record::operator==(Record const &r) const {
  return score==r.score
    && lines==r.lines
    && bricks==r.bricks
    && date==r.date;
}

bool Record::operator<(Record const &r) const {
  // highest score first, most lines first, earliest date first
  if (score > r.score)
    return true;
  else if (score < r.score)
    return false;
  else if (lines > r.lines)
    return true;
  else if (lines < r.lines)
    return false;
  else
    return date < r.date;
}

Record Record::fromJson(QJsonArray const &json) {
  Record r;
  QVariantList v = json.toVariantList();
  r.date = v[0].toString();
  r.score = v[1].toInt();
  r.lines = v[2].toInt();
  r.bricks = v[3].toInt();
  return r;
}

QJsonArray Record::toJson() const {
  QJsonArray json;
  json << date << score << lines << bricks;
  return json;
}

Records::Records() {
}

Records Records::fromJson(QJsonArray const &json) {
  Records rr;
  for (auto j: json)
    rr << Record::fromJson(j.toArray());
  return rr;
}
  
QJsonArray Records::toJson() const {
  QJsonArray ar;
  for (auto r: *this)
    ar << r.toJson();
  return ar;
}

int Records::add(Record const &r) {
  bool use = size() < MAXKEEP || r.score > last().score;
  if (use) {
    *this << r;
    std::sort(begin(), end());
    if (size() > MAXKEEP)
      removeLast();
    return indexOf(r);
  } else {
    return -1;
  }
}

AllRecords::AllRecords() {
}

AllRecords AllRecords::fromJson(QJsonObject const &json) {
  AllRecords rrr;
  for (auto name: json.keys()) {
    for (auto bskey: json[name].toObject().keys()) {
      int bs = bskey.toInt() - 1;
      rrr[name][bs] = Records::fromJson(json[name].toObject()[bskey].toArray());
    }
  }
  return rrr;
}
  
QJsonObject AllRecords::toJson() const {
  QJsonObject json;
  for (QString name: keys()) {
    QJsonObject json1;
    for (int bs: (*this)[name].keys()) {
      QString bskey(QString::number(bs + 1));
      json1[bskey] = (*this)[name][bs].toJson();
    }
    json[name] = json1;
  }
  return json;
}

void Records::report() {
  for (auto r: *this)
    qDebug() << "    " << r.score << r.lines << r.bricks << r.date;
}

int AllRecords::add(QString name, int bs,
                     int score, int lines, int bricks, QString date) {
  return (*this)[name][bs].add(Record(score, lines, bricks, date));
}

void AllRecords::report() {
  for (QString name: keys()) {
    qDebug() << name;
    for (int bs: (*this)[name].keys()) {
      qDebug() << "  " << bs;
      (*this)[name][bs].report();
    }
  }
}

AllRecords &AllRecords::instance() {
  static AllRecords rrr;
  static bool loaded = false;
  if (!loaded) 
    rrr = load();
  loaded = true;
  return rrr;
}

AllRecords AllRecords::load() {
  QDir localdata(Paths::datadir());
  QFile fd(localdata.filePath("records.json"));
  if (!fd.open(QFile::ReadOnly)) {
    qDebug() << "cannot read records";
    return AllRecords();
  }
  QJsonObject json(QJsonDocument::fromJson(fd.readAll()).object());
  return fromJson(json);
}

void AllRecords::save() const {
  QJsonObject json = toJson();
  QDir localdata(Paths::datadir());
  QFile fd(localdata.filePath("records.json"));
  if (fd.open(QFile::WriteOnly)) {
    fd.write(QJsonDocument(json).toJson());
  } else {
    throw "Cannot write records";
  }
}

void AllRecords::importScores(QString scorefn) {
  QFile fd(scorefn);
  if (!fd.open(QFile::ReadOnly)) {
    qDebug() << "cannot open" << scorefn;
    return;
  }

  while (!fd.atEnd()) {
    QStringList line = QString(fd.readLine()).trimmed().split("\t");
    qDebug() << "score" << line;
    if (line.size()) 
      add(line[5], line[1].toInt() - 1,
          line[2].toInt(), line[3].toInt(), line[4].toInt(), line[0]);
  }
}
