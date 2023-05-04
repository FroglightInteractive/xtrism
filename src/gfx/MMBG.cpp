// MMBG.C

#include "MMBG.h"
#include "MMPict.h"
#include "MainWindow.h"
#include <QFileInfo>
#include <QDebug>
#include "RGBImage.h"

MMBG::MMBG(QSize size, QString filename) {
  if (QFileInfo(filename).exists()) {
    pm.load(filename);
  } else {
    // we'll have to build it from scratch...
    qDebug() << "Making main menu background (this may take a while)\n";
    RGBImage mmp(mmPict(size.width(), size.height()));
    pm = QPixmap::fromImage(mmp.toQImage());
    if (!filename.isEmpty())
      mmp.save(filename);
  }
}

QPixmap const &MMBG::toPixmap() const {
  return pm;
}

