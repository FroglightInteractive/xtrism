// MainWindow.cpp

#include "MainWindow.h"
#include <QApplication>
#include <QFont>
#include <QScreen>

MainWindow::MainWindow(TReso const &reso) {
  Q_ASSERT(qApp);
  QSize screensize = qApp->primaryScreen()->size();
  bool full = reso.fullscreen();
  actfactor = reso.best(screensize.width(), screensize.height());
  
  QFont fnt("Albertus Medium");
  fnt.setPixelSize(actfactor*0.3);
  fnt.setWeight(QFont::Bold);
  setFont(fnt);

  wid = full ? screensize.width() : reso.actx(actfactor);
  hei = full ? screensize.height() : reso.acty(actfactor);

  resize(wid, hei);
  if (full) {
    showFullScreen();
    // setWindowState(widget->windowState() | Qt::WindowFullScreen);
  } else {
    move((screensize.width() - wid) / 2, (screensize.height() - hei) / 2);
    show();
  }

  qDebug() << "mainwindow" << wid << hei << actfactor;
}

MainWindow::~MainWindow() {
}

QString MainWindow::id() const {
  return QString("%1x%2-%3").arg(wid).arg(hei).arg(actfactor);
}

int MainWindow::actualfactor() const {
  return actfactor;
}
