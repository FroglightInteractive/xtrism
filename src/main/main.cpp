// main.C

#include "MainMenu.h"
#include "MainWindow.h"
#include <QApplication>
#include "TextButton.h"
#include "Paths.h"
#include "Sounds.h"
#include "SPlayerPA.h"
#include <QDateTime>
#include <QDebug>
#include "Records.h"

MainMenu *mmp = 0;
void setlastscore(int sc, int li, int brk, QString name, int bs) {
  if (sc > 0) {
    QStringList fields{
      QDateTime::currentDateTime().toString("yyyyMMdd.hhmmss"),
      QString::number(bs+1),
      QString::number(sc),
      QString::number(li),
      QString::number(brk),
      name};
    QString s = Paths::datadir();
    s += "/scores";
    QFile f(s);
    f.open(QFile::Append);
    f.write(fields.join("\t").toUtf8());
    f.write("\n");
  }

  if (mmp) {
    QString buf = QString("%1: %2 in %3 lines (%4 ppb); bset %5")
      .arg(name)
      .arg(sc)
      .arg(li)
      .arg(brk?sc*1.0/brk : 0.0, 0, 'f', 1)
      .arg(bs + 1);  
    mmp->setLastScore(buf);
  }
}


int main(int argc, char **argv) {
  QApplication app(argc, argv);
  app.setApplicationName("trism");
  qDebug() << "start of main";

  QDir(Paths::datadir()).mkpath(".");
  QDir(Paths::cachedir()).mkpath(".");

  //  AllRecords &recs(AllRecords::instance());
  //recs.importScores(Paths::datadir() + "/scores");
  //recs.report();
  //  recs.save();
  
  MainWindow mw(-1); // or put in max factor
  MainMenu mm(&mw);
  mmp = &mm; // for setlastscore; to be improved
  mm.show();
  mm.setFocus();
  SPlayerPA splayerpa;
  SPlayer::setInstance(&splayerpa);
  Sounds::instance();
  int r = app.exec();
  return r;
}
