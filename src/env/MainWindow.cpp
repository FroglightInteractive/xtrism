// MainWindow.cpp

#include "MainWindow.h"
#include <QApplication>
#include <QFont>
#include <QScreen>

MainWindow::MainWindow(TReso const &reso, QDir cachedir):
  cachedir_(cachedir) {
  Q_ASSERT(qApp);
  QSize screensize = qApp->primaryScreen()->size();
  bool full = reso.fullscreen();
  actfactor = reso.best(screensize.width(), screensize.height());
  
  QFont fnt("Charter");
  fnt.setPixelSize(actfactor*1.0);
  fnt.setWeight(QFont::Bold);
  setFont(fnt);

  int wid = full ? screensize.width() : reso.actx(actfactor);
  int hei = full ? screensize.height() : reso.acty(actfactor);

  if (full) {
    showFullScreen();
    // setWindowState(widget->windowState() | Qt::WindowFullScreen);
  } else {
    resize(wid, hei);
    move((screensize.width() - wid) / 2, (screensize.height() - hei) / 2);
    show();
  }
}
