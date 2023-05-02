// Solo, Team, Apart sessions

#ifndef _NiceSession_H
#define _NiceSession_H

#include "RGBImage.h"
#include "Rainbow.h"
#include <QWidget>

class NiceSession: public QWidget {
public:
  NiceSession(QString id,
              class Player const &p1,
              int bset,
              class MainWindow *mw, QWidget *playbutton); // solo
  NiceSession(QString id,
              class Player const &p1, class Player const &p2,
              int bset,
              class MainWindow *mw, QWidget *playbutton); // Team
  NiceSession(QString id,
              class Player const &p1, class Player const &p2,
              int bset1, int bset2,
              class MainWindow *mw, QWidget *playbutton); // Apart
  virtual ~NiceSession();
  void paintEvent(QPaintEvent *) override;
  void exec();
  Rainbow const &background() const;
  void keyPressEvent(QKeyEvent*) override;
  void keyReleaseEvent(QKeyEvent*) override;
private:
  NiceSession(QString id, MainWindow *mw, QWidget *playbutton);
  //  void init(QString id);
  //  void start();
  //  void gquit(class Game *g, bool quitable);
private:
  Rainbow bg_;
  QPixmap bg;
  QWidget *mm;
  class NiceGame *g1, *g2;
};

#endif
