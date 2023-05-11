// Records.h

#ifndef RECORDS_H

#define RECORDS_H

#include <QString>
#include <QJsonObject>

class Record {
public:
  QString date;
  int score;
  int lines;
  int bricks;
public:
  Record() { }
  Record(int score, int lines, int bricks, QString date="");
  bool operator<(Record const &r) const;
  bool operator==(Record const &r) const;
  static Record fromJson(QJsonArray const &);
  QJsonArray toJson() const;
};

class Records: public QList<Record> {
public:
  static constexpr int MAXKEEP = 20;
public:
  Records();
  static Records fromJson(QJsonArray const &);
  QJsonArray toJson() const;
  int add(Record const &); // returns rank, or -1 if not ranked
  void report();
};

class AllRecords: public QMap<QString, QMap<int, Records>> {
  // maps name -> bs (1..4) -> table of records
 public:
  AllRecords();
  static AllRecords fromJson(QJsonObject const &);
  static AllRecords load();
  QJsonObject toJson() const;
  void save() const;
  int add(QString name, int bs,
           int score, int lines, int bricks, QString date="");
  void report();
  void importScores(QString scorefn);
  static AllRecords &instance();
};

#endif
