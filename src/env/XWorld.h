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
  ID storePixmap(RGBImage &);
  void renderPixmap(ID id, int x, int y);
  void renderPixmap(ID id, int x, int y, QRect src);
  void flush();
private:
  class XWorldPrivate *d;
  QMap<ID, QSize> sizes;
};

#endif
