// MainMenu.H

#include <QWidget>
#include "Options.h"

class MainMenu: public QWidget {
public:
  MainMenu(class MainWindow *parent);
  ~MainMenu();
  void paintEvent(QPaintEvent *) override;
  void keyPressEvent(QKeyEvent *) override;
private:
  int currentPlayButton() const;
  void selectPlayer(Options::PPos which, int id);
  void selectBrickset(Options::PPos which, int id);
  void makePlayButtons();
  void makePlayerSelectors();
  void makeBricksetSelectors();
  void editKeys(Options::PPos which);
private:
  MainWindow *mw;
  QList<class PlayButton *> playbuttons;
  QMap<Options::PPos, class PlayerSelector *> playerselectors;
  QMap<Options::PPos, class BricksetSelector *> bricksetselectors;
  class MMBG *backg;
};
