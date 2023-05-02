// MMBG.C

#include "MMBG.h"
#include "MMPict.h"
#include "MainWindow.h"
#include <QFileInfo>
#include <QDebug>
#include "RGBMap.h"

MMBG::MMBG(QSize size, QString filename) {
  if (QFileInfo(filename).exists()) {
    pm.load(filename);
  } else {
    // we'll have to build it from scratch...
    qDebug() << "Making main menu background (this may take a while)\n";
    MMPict mmp(size.width(), size.height());
    pm = QPixmap::fromImage(*mmp.rgbmap());
    if (!filename.isEmpty())
      mmp.rgbmap()->save(filename);
  }
}

QPixmap const &MMBG::toPixmap() const {
  return pm;
}

