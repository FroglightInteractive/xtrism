// PlayButton.H

#ifndef _PlayButton_H
#define _PlayButton_H

#include "../gfx/TextButton.h"
#include "Options.h"

class PlayButton: public TextButton {
public:
  PlayButton(QString text, QWidget *parent=0);
  virtual ~PlayButton();
  void mousePressEvent(QMouseEvent *) override;
  void select();
  void deselect();
  bool isSelected() const;
  virtual void play() = 0;
private:
  QString basetext;
  bool sel;
  QWidget *par;
};

class SoloButton: public PlayButton {
public:
  SoloButton(QString text, Options::PPos pos, QWidget *parent=0);
  void play() override;
private:
  Options::PPos pos;
};

class TeamButton: public PlayButton {
public:
  TeamButton(QString text, QWidget *parent=0);
  void play() override;
};

class ApartButton: public PlayButton {
public:
  ApartButton(QString text, QWidget *parent=0);
  void play() override;
};

#endif
