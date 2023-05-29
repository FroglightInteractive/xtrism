// NewPlayer.h

#ifndef NEWPLAYER_H

#define NEWPLAYER_H

#include "MarbledButton.h"

class NewPlayer: public MarbledButton {
  Q_OBJECT;
public:
  NewPlayer(QWidget *parent=0);
  ~NewPlayer();
  QString name() const;
  bool exec();
  void keyPressEvent(QKeyEvent *) override;
private:
  void keyPress(QString txt, int qkey);
  void rebuild();
signals:
  void cancel(); // private
  void accept(); // private
private:
  class TranspLabel *editor_;
  QString name_;
  int pos_;
};

#endif
