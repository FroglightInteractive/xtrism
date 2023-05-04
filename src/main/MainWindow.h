// MainWindow.h

#ifndef MAINWINDOW_H

#define MAINWINDOW_H

#include <QWidget>
#include <QDir>

class MainWindow: public QWidget {
public:
  MainWindow(int maxf=-1); // -1 for full screen, or positive for limit
  ~MainWindow();
  class BrickSprites const *brickSprites(bool striped=false);
  QString id() const;
  class XWorld *xworld();
private:
  int wid, hei;
  BrickSprites *bspr[2];
  XWorld *xw;
};

#endif
