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
  Record(class Score const &);
  bool operator<(Record const &r) const;
  bool operator==(Record const &r) const;
  bool operator!=(Record const &r) const { return !(*this==r); }
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
  void dropBefore(QString datepfx);
  void report();
};

class AllRecords: public QMap<QString, QMap<int, Records>> {
  // maps name -> bs (1..4) -> table of records
public:
  enum class Era {
    AllTime,
    ThisYear,
    Today
  };
 public:
  AllRecords();
  static AllRecords fromJson(QJsonObject const &);
  static AllRecords load(AllRecords::Era e);
  QJsonObject toJson() const;
  void save(AllRecords::Era e) const;
  int add(QString name, int bs, Record const &);
  void report();
  void dropBefore(QString datepfx);
  void importScores(QString scorefn);
  static AllRecords &instance(Era e=Era::AllTime);
  static int addAndSave(QString name, int bs, Record const &rec);
private:
  static QString filename(AllRecords::Era e);
};

#endif
