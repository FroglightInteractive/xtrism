// XWorld.h

#ifndef XWORLD_H

#define XWORLD_H

#include <cstdint>
#include <QApplication>
#include <QWidget>
#include <QList>
#include "RGBImage.h"

class XWorld {
public:
  typedef std::uint64_t ID;
public:
  XWorld(QApplication *app, QWidget *widget);
  ~XWorld();
  bool isActive() const;
  void storePixmap(QPixmap const *);
  void dropPixmap(QPixmap const *);
  void renderPixmap(QPixmap const *pm, int x, int y);
  void renderPixmap(QPixmap const *pm, int x, int y, QRect src);
  void flush();
private:
  class XWorldPrivate *d;
  QMap<QPixmap const *, ID> pixmaps;
  QMap<ID, QSize> sizes;
};

#endif
