// Records.cpp

#include "Records.h"
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QVariantList>
#include <QDir>
#include "Paths.h"
#include <QSet>
#include "Score.h"

Record::Record(Score const &s): Record(s.score(), s.lines(), s.bricks()) {
}

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

void Records::dropBefore(QString datepfx) {
  auto pred = [datepfx](Record const &r) { return r.date < datepfx; };
  erase_if(*this, pred);
//  iterator it1;
//  for (iterator it=begin(); it!=end(); it=it1) {
//    it1 = it;
//    it1++;
//    if ((*it).date < datepfx)
//      erase(it);
//  }
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

void AllRecords::dropBefore(QString datepfx) {
  for (QString name: keys()) 
    for (int bs: (*this)[name].keys())
      (*this)[name][bs].dropBefore(datepfx);
}

void Records::report() {
  for (auto r: *this)
    qDebug() << "    " << r.score << r.lines << r.bricks << r.date;
}

int AllRecords::add(QString name, int bs, Record const &rec) {
  return (*this)[name][bs].add(rec);
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

AllRecords &AllRecords::instance(AllRecords::Era e) {
  static QMap<Era, AllRecords> rrr;
  static QSet<Era> loaded;
  if (!loaded.contains(e))
    rrr[e] = load(e);
  loaded << e;
  return rrr[e];
}

QString AllRecords::filename(AllRecords::Era e) {
  static QMap<Era, QString> fns{
    {Era::AllTime, "records"},
    {Era::ThisYear, "thisyear"},
    {Era::Today, "today"}};
  return fns[e];
}

AllRecords AllRecords::load(AllRecords::Era e) {
  QDir localdata(Paths::datadir());
  QFile fd(localdata.filePath(filename(e) + ".json"));
  if (!fd.open(QFile::ReadOnly)) {
    qDebug() << "cannot read records";
    return AllRecords();
  }
  QJsonObject json(QJsonDocument::fromJson(fd.readAll()).object());
  AllRecords rr = fromJson(json);
  if (e==Era::ThisYear) 
    rr.dropBefore(QDateTime::currentDateTime().toString("yyyy"));
  else if (e==Era::Today)
    rr.dropBefore(QDateTime::currentDateTime().toString("yyyyMMdd"));
  return rr;
}

void AllRecords::save(AllRecords::Era e) const {
  QJsonObject json = toJson();
  QDir localdata(Paths::datadir());
  QFile fd(localdata.filePath(filename(e) + ".json"));
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
          Record(line[2].toInt(), line[3].toInt(), line[4].toInt(), line[0]));
  }
}

int AllRecords::addAndSave(QString name, int bs, Record const &rec) {
  QMap<Era, int> ranks;
  for (Era e: QList<Era>{Era::AllTime, Era::ThisYear, Era::Today}) {
    ranks[e] = instance(e).add(name, bs, rec);
    if (ranks[e] >= 0)
      instance(e).save(e);
  }
  return ranks[Era::AllTime];
}
