// main.C

#include "MainMenu.h"
#include "MainWindow.h"
#include <QApplication>
#include "TextButton.h"
#include "Paths.h"

MainMenu *mmp = 0;
void setlastscore(int sc, int li, double ppb, QString name, int bs) {
  QString buf = QString("%1: %2 in %3 lines (%4 ppb); bset %5")
    .arg(name).arg(sc).arg(li).arg(ppb, 0, 'f', 1).arg(bs + 1);
  QDateTime dt(QDateTime::currentDateTime());
  buf += dt.toString(" - MMMM d yyyy hh:mm");
  if (sc > 0) {
    QString s = Paths::datadir();
    s += "/hisc";
    QFile f(s);
    f.open(QFile::Append);
    f.write(buf.toUtf8());
    f.write("\n");
  }
  mmp->setLastScore(buf);
}


int main(int argc, char **argv) {
  QApplication app(argc, argv);
  app.setApplicationName("trism");
  qDebug() << "start of main";

  QDir(Paths::datadir()).mkpath(".");
  QDir(Paths::cachedir()).mkpath(".");
  
  MainWindow mw(-1); // or put in max factor
  MainMenu mm(&mw);
  mmp = &mm; // for setlastscore; to be improved
  mm.show();
  mm.setFocus();
  int r = app.exec();
  return r;
}
