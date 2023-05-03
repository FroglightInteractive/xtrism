// PlayButton.C

#include "PlayButton.h"
#include "NiceSession.h"

PlayButton::PlayButton(QString text, MainWindow *mw, QWidget *parent):
  TextButton(parent), mw(mw) {
  sel = false;
  basetext = text;
  setText(basetext);
}

PlayButton::~PlayButton() {
}

void PlayButton::select() {
  sel = true;
  setText("[" + basetext + "]");
  emit selected(this);
}

void PlayButton::deselect() {
  sel = false;
  setText(basetext);
}

bool PlayButton::isSelected() const {
  return sel;
}

void PlayButton::mousePressEvent(QMouseEvent *) {
  if (isSelected())
    play();
  else
    select();
}

SoloButton::SoloButton(QString text, Options::PPos pos,
                       MainWindow *mw, QWidget *parent):
  PlayButton(text, mw, parent), pos(pos) {
}

void SoloButton::play() {
  NiceSession session(pos==Options::PPos::Left ? "1" : "4",
                      Options::instance().currentPlayer(pos),
                      Options::instance().currentBrickset(pos),
                      mw, this);
  session.exec();
}

TeamButton::TeamButton(QString text,
                       MainWindow *mw, QWidget *parent):
  PlayButton(text, mw, parent) {
}

void TeamButton::play() {
  NiceSession session("2",
                      Options::instance().currentPlayer(Options::PPos::Left),
                      Options::instance().currentPlayer(Options::PPos::Right),
                      Options::instance().currentBrickset(Options::PPos::Left),
                      mw, this);
  session.exec();
}

ApartButton::ApartButton(QString text,
                         MainWindow *mw, QWidget *parent):
  PlayButton(text, mw, parent) {
}

void ApartButton::play() {
  NiceSession session("3",
                      Options::instance().currentPlayer(Options::PPos::Left),
                      Options::instance().currentPlayer(Options::PPos::Right),
                      Options::instance().currentBrickset(Options::PPos::Left),
                      Options::instance().currentBrickset(Options::PPos::Right),
                      mw, this);
  session.exec();
}
