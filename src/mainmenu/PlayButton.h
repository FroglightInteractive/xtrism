// PlayButton.H

#ifndef _PlayButton_H
#define _PlayButton_H

#include "../gfx/TextButton.h"
#include "Options.h"

class PlayButton: public TextButton {
  Q_OBJECT;
public:
  PlayButton(QString text, class MainWindow *mw, QWidget *parent=0);
  virtual ~PlayButton();
  void mousePressEvent(QMouseEvent *) override;
  void select();
  void deselect();
  bool isSelected() const;
  virtual void play() = 0;
signals:
  void selected(QWidget *me);
private:
  QString basetext;
  bool sel;
protected:
  MainWindow *mw;
};

class SoloButton: public PlayButton {
public:
  SoloButton(QString text, Options::PPos pos,
             class MainWindow *mw, QWidget *parent=0);
  void play() override;
private:
  Options::PPos pos;
};

class TeamButton: public PlayButton {
public:
  TeamButton(QString text, class MainWindow *mw, QWidget *parent=0);
  void play() override;
};

class ApartButton: public PlayButton {
public:
  ApartButton(QString text, class MainWindow *mw, QWidget *parent=0);
  void play() override;
};

#endif
