// NextBox.H

#ifndef _NextBox_H
#define _NextBox_H

#include <QWidget>
#include "RGBMap.h"
#include <QPixmap>

class NextBox: public QWidget {
public:
  NextBox(class SBrickData const &sbd0,
          class BrickSprites const &bs0,
          RGBMap const &sharedbg, QWidget *parent);
  ~NextBox();
  void setbrick(int bno0, int rot0);
  void poll();
  void clear() {
    setbrick(-1, 0);
  }
  void paintEvent(QPaintEvent *) override;
private:
  void generate();
private:
  class SBrickData const &sbd;
  class BrickSprites const &bs;
  RGBMap const &sharedbg;
  int bno, rot;
  QPixmap mybg;
  QPoint topleft;
};

#endif
