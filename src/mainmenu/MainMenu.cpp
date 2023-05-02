// MainMenu.C

#include "MainMenu.h"

#include "../globals/Globals.h"
#include "../pics/MMBG.h"
#include "../basics/minmax.h"
#include "PlayButton.h"
#include <QKeyEvent>
#include "../basics/dbx.h"
#include "MainWindow.h"
#include <QPainter>
#include "Globals.h"

MainMenu::MainMenu(): QWidget(mainwindow()) {
  backg = 0;
  resize(mainwindow()->size());
  qDebug() << "mm" << mainwindow()->size() << size();
  QSize pbarea(mainwindow()->actualfactor() * 3, mainwindow()->actualfactor() * 2);
  playbuttons << new SoloButton("Solo Play", Options::PPos::Left, this);
  playbuttons << new TeamButton("Team Play", this);
  playbuttons << new ApartButton("Separate Play", this);
  playbuttons << new SoloButton("Solo Play", Options::PPos::Right, this);
  int space = width() - 4*pbarea.width();
  for (int k=0; k<4; k++)
    playbuttons[k]->setGeometry(QRect(QPoint(space*(k+1)/5 + pbarea.width()*k,
                                             height()-1.85*pbarea.height()),
                                      pbarea));

      
  textbut = new TextButton(this);
  textbut->setGeometry(QRect(QPoint(width()*1/8, height()*1/10),
                             QSize(width()*3/4, height()/24)));
  textbut->setText("X T R I S M");

  playbuttons[0]->select();
}

MainMenu::~MainMenu() {
  dbx(1, "~MainMenu()");
  delete backg;
}

void MainMenu::paintEvent(QPaintEvent *) {
  qDebug() << "mm" << size();
  QPainter p(this);
  if (!backg)
    backg = new MMBG(mainwindow()->size(),
                     cachedir() + "/mmbg-" + mainwindow()->id() + ".jpg");
  p.drawPixmap(0, 0, backg->toPixmap());
}

void MainMenu::setLastScore(QString s) {
  textbut->setText(s);
}

int MainMenu::currentPlayButton() const {
  for (int k=0; k<playbuttons.size(); ++k)
    if (playbuttons[k]->isSelected())
      return k;
  return 0; // fallback
}

void MainMenu::keyPressEvent(QKeyEvent *e) {
  switch (e->key()) {
  case Qt::Key_1:
    options().setCurrentBrickset(Options::PPos::Left, 0);
    options().setCurrentBrickset(Options::PPos::Right, 0);
    break;
  case Qt::Key_2:
    options().setCurrentBrickset(Options::PPos::Left, 1);
    options().setCurrentBrickset(Options::PPos::Right, 1);
    break;
  case Qt::Key_3:
    options().setCurrentBrickset(Options::PPos::Left, 2);
    options().setCurrentBrickset(Options::PPos::Right, 2);
    break;
  case Qt::Key_4:
    options().setCurrentBrickset(Options::PPos::Left, 3);
    options().setCurrentBrickset(Options::PPos::Right, 3);
    break;
  case Qt::Key_Space:
  case Qt::Key_Enter:
  case Qt::Key_Return:
    playbuttons[currentPlayButton()]->play();
    break;
  case Qt::Key_Right:
    playbuttons[(currentPlayButton()+1)%playbuttons.size()]
      ->select();
    break;
  case Qt::Key_Left:
    playbuttons[(currentPlayButton()+playbuttons.size()-1)%playbuttons.size()]
      ->select();
    break;
  }
}
