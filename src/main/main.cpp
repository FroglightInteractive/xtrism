// main.C

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "../globals/Globals.h"
#include "../mainmenu/MainMenu.h"
#include <exception>
#include <QApplication>
#include "../gfx/TextButton.h"
#include <QDir>

MainMenu *mmp;

void setlastscore(int sc, int li, double ppb, char const *name, int bs) {
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
  mmp->textbut->setText(buf);
}

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  int r = 0;
  qDebug() << "start of main";
  try {
    global_init(argc, argv, &app);
    mmp = new MainMenu(mainwindow());
    app.exec();
    delete mmp;
    global_destroy();
  } catch (QString &s) {
    qDebug() << "exc:main.string: " << s;
  } catch (char *s) {
    qDebug() << "exc:main.char*: " << s;
  } catch (...) {
    qDebug() << "exc:main....: unknown expection";
  }
  qDebug() << "end of main";
  return r;
}
