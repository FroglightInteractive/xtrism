// HighScoreScreen.h

#ifndef HIGHSCORESCREEN_H

#define HIGHSCORESCREEN_H

#include <QWidget>
#include "NiceGame.h"
#include <QElapsedTimer>

class HighScoreScreen: public QWidget {
  Q_OBJECT;
public:
  HighScoreScreen(NiceGame *game,
                  QPixmap bg, QWidget *parent);
  HighScoreScreen(NiceGame *game1, NiceGame *game2,
                  QPixmap bg, QWidget *parent);
  ~HighScoreScreen();
  void paintEvent(QPaintEvent *) override;
  void keyPressEvent(QKeyEvent *) override;
signals:
  void keyPressed();
private:
  void renderGame(QPainter &, NiceGame *game, int x, int w);
private:
  NiceGame *game1, *game2;
  QPixmap bg;
  QElapsedTimer t0;
};

#endif
