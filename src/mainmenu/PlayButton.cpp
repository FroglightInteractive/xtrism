// PlayButton.C

#include "PlayButton.h"

#include "../basics/dbx.h"
#include "../sessions/NiceSession.h"
#include "../globals/Globals.h"

static QMap<QWidget *, QSet<PlayButton*>> playbuttons;

PlayButton::PlayButton(QString text, QWidget *parent):
  TextButton(parent), par(parent) {
  sel = false;
  basetext = text;
  setText(basetext);
  playbuttons[par].insert(this);
}

PlayButton::~PlayButton() {
  playbuttons[par].remove(this);
}

void PlayButton::select() {
  for (auto pb: playbuttons[par])
    if (pb!=this)
      pb->deselect();
  sel = true;
  setText("[" + basetext + "]");
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
                       QWidget *parent):
  PlayButton(text, parent), pos(pos) {
}

void SoloButton::play() {
  NiceSession session(pos==Options::PPos::Left ? "1" : "4",
                      options().currentPlayer(pos),
                      options().currentBrickset(pos),
                      mainwindow(), this);
  session.exec();
}

TeamButton::TeamButton(QString text,
                       QWidget *parent):
  PlayButton(text, parent) {
}

void TeamButton::play() {
  NiceSession session("2",
                      options().currentPlayer(Options::PPos::Left),
                      options().currentPlayer(Options::PPos::Right),
                      options().currentBrickset(Options::PPos::Left),
                      mainwindow(), this);
  session.exec();
}

ApartButton::ApartButton(QString text,
                         QWidget *parent):
  PlayButton(text, parent) {
}

void ApartButton::play() {
  NiceSession session("3",
                      options().currentPlayer(Options::PPos::Left),
                      options().currentPlayer(Options::PPos::Right),
                      options().currentBrickset(Options::PPos::Left),
                      options().currentBrickset(Options::PPos::Right),
                      mainwindow(), this);
  session.exec();
}
