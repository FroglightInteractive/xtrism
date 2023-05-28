// MMBG.C

#include "MMBG.h"
#include "MMPict.h"
#include "MainWindow.h"
#include <QFileInfo>
#include <QDebug>
#include "RGBImage.h"
#include "Logo.h"
#include <QPainter>
#include "Paths.h"

MMBG::MMBG(QSize size) {
  QString id = QString("%1x%2").arg(size.width()).arg(size.height());
  QString fn = Paths::cachedir() + "/mmbg-" + id + ".jpg";
  if (QFileInfo(fn).exists()) {
    pm.load(fn);
    return;
  }

  QString fn0 = Paths::cachedir() + "/mmbg0-" + id + ".jpg";
  QImage img;
  if (QFileInfo(fn0).exists()) {
    img = QImage(fn0);
  } else {
    // we'll have to build it from scratch...
    qDebug() << "Making main menu background (this may take a while)\n";
    RGBImage mmp(mmPict(size.width(), size.height()));
    img = mmp.toQImage();
    img.save(fn0);
  }

  qDebug() << "Drawing logo";
  QRect r(QPoint(size.width()/6, size.height()/13),
          QSize(size.width()*2/3, size.height()/3));
  QImage logo(::logo(r, size));
  { QPainter p(&img);
    p.drawImage(r.center() - QPoint(logo.width(),logo.height())/2, logo);
  }
  pm = QPixmap::fromImage(img);
  img.save(fn);
  qDebug() << "Drawing done";
}

QPixmap const &MMBG::toPixmap() const {
  return pm;
}

