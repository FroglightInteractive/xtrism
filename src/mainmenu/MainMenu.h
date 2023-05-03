// MainMenu.H

#include <QWidget>

class MainMenu: public QWidget {
public:
  MainMenu(class MainWindow *parent);
  ~MainMenu();
  void paintEvent(QPaintEvent *) override;
  void setLastScore(QString);
  void keyPressEvent(QKeyEvent *) override;
private:
  int currentPlayButton() const;
private:
  MainWindow *mw;
  QList<class PlayButton *> playbuttons;
  class TextButton *textbut;
  class MMBG *backg;
  //  class BoxMarblers *bms;
  //  class InputField *inpf;
  //  class PlayerSelector *selectors[2];
};
