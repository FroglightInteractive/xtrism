// MainWindow.h

#ifndef MAINWINDOW_H

#define MAINWINDOW_H

#include <QWidget>
#include "TReso.h"
#include <QDir>

class MainWindow: public QWidget {
public:
  MainWindow(TReso const &reso, QDir cachedir);
  ~MainWindow();
public:
  int actualfactor() const;
  QDir cachedir() const;
private:
  int actfactor;
  QDir cachedir_;
};

#endif
