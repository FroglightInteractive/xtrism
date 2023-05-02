// MainWindow.h

#ifndef MAINWINDOW_H

#define MAINWINDOW_H

#include <QWidget>
#include "TReso.h"
#include <QDir>

class MainWindow: public QWidget {
public:
  MainWindow(TReso const &reso);
  ~MainWindow();
public:
  QString id() const;
  int actualfactor() const;
private:
  int actfactor;
  int wid, hei;
};

#endif
