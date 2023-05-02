// main.C

#include "MainMenu.h"

#include "MainWindow.h"
#include "Globals.h"
#include <QApplication>
#include "../gfx/TextButton.h"

MainMenu *mmp;

void setlastscore(int sc, int li, double ppb, QString name, int bs) {
  QString buf = QString("%1: %2 in %3 lines (%4 ppb); bset %5")
    .arg(name).arg(sc).arg(li).arg(ppb, 0, 'f', 1).arg(bs + 1);
  QDateTime dt(QDateTime::currentDateTime());
  buf += dt.toString(" - MMMM d yyyy hh:mm");
  if (sc > 0) {
    QString s = datadir();
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
  int r = 0;
  qDebug() << "start of main";
  try {
    global_init(argc, argv, &app);
    mmp = new MainMenu();
    mmp->show();
    mmp->setFocus();
    app.exec();
    delete mmp;
    global_destroy();
  } catch (QString const &s) {
    qDebug() << "exc:main.string: " << s;
  } catch (char const *s) {
    qDebug() << "exc:main.char*: " << s;
  } catch (...) {
    qDebug() << "exc:main....: unknown expection";
  }
  qDebug() << "end of main";
  return r;
}
