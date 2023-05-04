// MainMenu.C

#include "MainMenu.h"

#include "MMBG.h"
#include "minmax.h"
#include "PlayButton.h"
#include <QKeyEvent>
#include "dbx.h"
#include "MainWindow.h"
#include <QPainter>
#include "Paths.h"
#include "SPlayer.h"

MainMenu::MainMenu(MainWindow *mw): QWidget(mw), mw(mw) {
  backg = 0;
  resize(mw->size());
  playbuttons << new SoloButton("Solo Play", Options::PPos::Left, mw, this);
  playbuttons << new TeamButton("Team Play", mw, this);
  playbuttons << new ApartButton("Separate Play", mw, this);
  playbuttons << new SoloButton("Solo Play", Options::PPos::Right, mw, this);
  int pbw = width()/6;
  int pbh = height()/6;
  int space = width() - 4*pbw;
  for (int k=0; k<4; k++)
    playbuttons[k]->setGeometry(QRect(QPoint(space*(k+1)/5 + pbw*k,
                                             height()-1.85*pbh),
                                      QSize(pbw, pbh)));
  for (int k=0; k<4; k++)
    connect(playbuttons[k], &PlayButton::selected,
            this, [this](QWidget *pb) {
              for (int k=0; k<4; k++)
                if (playbuttons[k]!=pb)
                  playbuttons[k]->deselect();
            });
  
      
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
    backg = new MMBG(mw->size(),
                     Paths::cachedir() + "/mmbg-" + mw->id() + ".jpg");
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
  case Qt::Key_Q:
    SPlayer::instance()->toggleSounds();
    break;
  case Qt::Key_1:
    Options::instance().setCurrentBrickset(Options::PPos::Left, 0);
    Options::instance().setCurrentBrickset(Options::PPos::Right, 0);
    break;
  case Qt::Key_2:
    Options::instance().setCurrentBrickset(Options::PPos::Left, 1);
    Options::instance().setCurrentBrickset(Options::PPos::Right, 1);
    break;
  case Qt::Key_3:
    Options::instance().setCurrentBrickset(Options::PPos::Left, 2);
    Options::instance().setCurrentBrickset(Options::PPos::Right, 2);
    break;
  case Qt::Key_4:
    Options::instance().setCurrentBrickset(Options::PPos::Left, 3);
    Options::instance().setCurrentBrickset(Options::PPos::Right, 3);
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
