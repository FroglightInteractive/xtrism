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
#include "PlayerSelector.h"
#include "BricksetSelector.h"
#include "KeyEditor.h"
#include "NewPlayer.h"

MainMenu::MainMenu(MainWindow *mw): QWidget(mw), mw(mw) {
  backg = 0;
  resize(mw->size());

  makePlayButtons();
  makePlayerSelectors();
  makeBricksetSelectors();
}

void MainMenu::makePlayButtons() {
  playbuttons << new SoloButton("Solo Play", Options::PPos::Left, mw, this);
  playbuttons << new TeamButton("Team Play", mw, this);
  playbuttons << new ApartButton("Separate Play", mw, this);
  playbuttons << new SoloButton("Solo Play", Options::PPos::Right, mw, this);

  int pbw = width()/6;
  int pbh = height()/6;
  int space = width() - 4*pbw;
  for (int k=0; k<4; k++)
    playbuttons[k]->setGeometry(QRect(space*(k+1)/5 + pbw*k,
                                      height()-1.8*pbh
                                      +.1*pbh*(k==1 || k==2),
                                      pbw,
                                      pbh));
  for (int k=0; k<4; k++)
    connect(playbuttons[k], &PlayButton::selected,
            this, [this](QWidget *pb) {
              for (int k=0; k<4; k++)
                if (playbuttons[k]!=pb)
                  playbuttons[k]->deselect();
            });
  
  playbuttons[0]->select();
}

void MainMenu::makePlayerSelectors() {
  int pbw = width()/6;
  int space = width() - 4*pbw;
  int x0 = space/8;
  int psw = .2*width();
  int psh = height()/24;
  int y0 = .52*height();
  int x1 = width() - x0 - psw;
  playerselectors[Options::PPos::Left] = new PlayerSelector(this);
  playerselectors[Options::PPos::Right] = new PlayerSelector(this);
  connect(playerselectors[Options::PPos::Left], &PlayerSelector::playerChanged,
          this, [this](int id) { selectPlayer(Options::PPos::Left, id); });
  connect(playerselectors[Options::PPos::Right], &PlayerSelector::playerChanged,
          this, [this](int id) { selectPlayer(Options::PPos::Right, id); });
  playerselectors[Options::PPos::Left]->setGeometry(QRect(x0, y0, psw, psh));
  playerselectors[Options::PPos::Right]->setGeometry(QRect(x1, y0, psw, psh));
  playerselectors[Options::PPos::Left]
    ->selectPlayer(Options::instance().currentPlayerID(Options::PPos::Left));
  playerselectors[Options::PPos::Right]
    ->selectPlayer(Options::instance().currentPlayerID(Options::PPos::Right));
  connect(playerselectors[Options::PPos::Left], &PlayerSelector::gearPressed,
          this, [this]() { editKeys(Options::PPos::Left); });
  connect(playerselectors[Options::PPos::Right], &PlayerSelector::gearPressed,
          this, [this]() { editKeys(Options::PPos::Right); });
  connect(playerselectors[Options::PPos::Left], &PlayerSelector::newPlayer,
          this, [this]() { newPlayer(Options::PPos::Left); });
  connect(playerselectors[Options::PPos::Right], &PlayerSelector::newPlayer,
          this, [this]() { newPlayer(Options::PPos::Right); });
}

void MainMenu::makeBricksetSelectors() {
  int pbw = width()/6;
  int space = width() - 4*pbw;
  int x0 = space/6;
  int bsw = .16*width();
  int bsh = height()/24;
  int y0 = .58*height();
  int x1 = width() - x0 - bsw;
  bricksetselectors[Options::PPos::Left] = new BricksetSelector(this);
  bricksetselectors[Options::PPos::Right] = new BricksetSelector(this);
  connect(bricksetselectors[Options::PPos::Left],
          &BricksetSelector::bricksetChanged,
          this, [this](int id) { selectBrickset(Options::PPos::Left, id); });
  connect(bricksetselectors[Options::PPos::Right],
          &BricksetSelector::bricksetChanged,
          this, [this](int id) { selectBrickset(Options::PPos::Right, id); });
  bricksetselectors[Options::PPos::Left]->setGeometry(QRect(x0, y0, bsw, bsh));
  bricksetselectors[Options::PPos::Right]->setGeometry(QRect(x1, y0, bsw, bsh));
  bricksetselectors[Options::PPos::Left]
    ->selectBrickset(Options::instance().currentBrickset(Options::PPos::Left));
  bricksetselectors[Options::PPos::Right]
    ->selectBrickset(Options::instance().currentBrickset(Options::PPos::Right));
}

MainMenu::~MainMenu() {
  dbx(1, "~MainMenu()");
  delete backg;
}

void MainMenu::paintEvent(QPaintEvent *) {
  qDebug() << "mm" << size();
  QPainter p(this);
  if (!backg)
    backg = new MMBG(mw->size());
  p.drawPixmap(0, 0, backg->toPixmap());
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

void MainMenu::selectPlayer(Options::PPos which, int id) {
  Options &options(Options::instance());
  if (options.currentPlayerID(which)==id)
    return;
  Options::PPos other = Options::otherPos(which);
  if (options.currentPlayerID(other)==id) {
    options.setCurrentPlayer(other, options.currentPlayerID(which)); // swap
    playerselectors[other]->selectPlayer(options.currentPlayerID(other));
  }
  options.setCurrentPlayer(which, id);
  options.save();
}

void MainMenu::selectBrickset(Options::PPos which, int id) {
  Options &options(Options::instance());
  if (options.currentBrickset(which)==id)
    return;
  options.setCurrentBrickset(which, id);
  options.save();
}

void MainMenu::editKeys(Options::PPos which) {
  KeyEditor ke(Options::instance().currentPlayer(which), mw);
  hide();
  ke.exec();
  show();
}

void MainMenu::newPlayer(Options::PPos which) {
  NewPlayer np(this);
  np.move(width()/2 - np.width()/2,
           height()/2 - np.height()*.7);
  if (!np.exec())
    return;
  
  qDebug() << "New player!" << np.name();
  int id = 1;
  for (int pid: Options::instance().allPlayerIDs())
    if (pid>=id)
      id = pid+1;
  Player player;
  player.setID(id);
  player.setName(np.name());
  Options::instance().updatePlayer(player);
  Options::instance().save();
  Options::PPos other = Options::otherPos(which);
  int oid = playerselectors[other]->currentPlayer();
  playerselectors[Options::PPos::Left]->rebuild();
  playerselectors[Options::PPos::Right]->rebuild();
  playerselectors[which]->selectPlayer(id);
  playerselectors[other]->selectPlayer(oid);
  
}
