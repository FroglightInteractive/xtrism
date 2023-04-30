// MainMenu.H

#include <QWidget>

class MainMenu: public QWidget {
public:
  MainMenu(class MainWindow *mw);
  ~MainMenu();
  void paintEvent(QPaintEvent *) override;
public:
  class TextButton *playbuttons[4], *textbut;
private:
  class MMBG *backg;
  class BoxMarblers *bms;
  class InputField *inpf;
  class PlayerSelector *selectors[2];
};
