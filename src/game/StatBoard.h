// StatBoard.H

#ifndef _StatBoard_H
#define _StatBoard_H

#include <QWidget>
#include "Rainbow.h"
#include <QPixmap>

class StatBoard: public QWidget {
public:
  StatBoard(int nlines, int labelw, int dataw,
            Rainbow const &sbg, QWidget *parent=0);
  void setlabel(int i, QString const &txt, bool update=true);
  void setdata(int i, QString const &txt, bool update=true);
  void setdata(int i, int val, bool update=true);
  void setdata(int i, double val, bool update=true);
  void cleardata();
  void paintEvent(QPaintEvent *) override;
private:
  void generate();
  void ensure(int);
private:
  Rainbow const &sharedbg;
  QStringList labels;
  QStringList data;
  int y0, dy;
  int dxl, dxd;
  QPoint topleft;
  QPixmap mybg;
};
#endif
