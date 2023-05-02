// MainMenu.H

#include <QWidget>

class MainMenu: public QWidget {
public:
  MainMenu();
  ~MainMenu();
  void paintEvent(QPaintEvent *) override;
  void setLastScore(QString);
private:
  QList<class PlayButton *> playbuttons;
  class TextButton *textbut;
  class MMBG *backg;
  //  class BoxMarblers *bms;
  //  class InputField *inpf;
  //  class PlayerSelector *selectors[2];
};
