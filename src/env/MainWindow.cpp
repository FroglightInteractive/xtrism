// MainWindow.cpp

#include "MainWindow.h"
#include <QApplication>
#include <QFont>
#include <QScreen>
#include "minmax.h"
#include "Paths.h"
#include "XWorld.h"
#include "BrickData.h"
#include "BrickSprites.h"

MainWindow::MainWindow(int maxf) {
  Q_ASSERT(qApp);

  constexpr int x = 16;
  constexpr int y = 9;
  QSize screensize = qApp->primaryScreen()->size();
  wid = screensize.width();
  hei = screensize.height();
  if (maxf>0) {
    int factor = std::min(maxf, std::min(wid/x, hei/y));
    wid = factor*x;
    hei = factor*y;
  }

  //  QFont fnt("Benguiat Bk BT");
  //  QFont fnt("Albertus Medium");
  QFont fnt("Charter");
  fnt.setPixelSize(0.27*wid/16);
  fnt.setWeight(QFont::Bold);
  setFont(fnt);

  resize(wid, hei);
  if (maxf<0) 
    showFullScreen();
  else 
    show();

  xw = new XWorld(qApp, this);
  if (!xw->isActive()) {
    delete xw;
    xw = 0;
  }

  for (int striped=0; striped<2; striped++)
    bspr[striped] =  new BrickSprites(SBrickData::instance(),
                                      Paths::cachedir(),
                                      wid/40, striped);

  if (xw) {
    // import bricksprites into xworld
  }
}


MainWindow::~MainWindow() {
  delete xw;
  delete bspr[0];
  delete bspr[1];
}


QString MainWindow::id() const {
  return QString("%1x%2").arg(wid).arg(hei);
}


BrickSprites const *MainWindow::brickSprites(bool striped) {
  return bspr[striped ? 1 : 0];
}
