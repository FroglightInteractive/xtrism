// PlayerSelector.h

#ifndef PLAYERSELECTOR_H

#define PLAYERSELECTOR_H

#include "ComboBox.h"

class PlayerSelector: public ComboBox {
  Q_OBJECT;
public:
  PlayerSelector(QWidget *parent=0);
  ~PlayerSelector();
  bool selectPlayer(int id);
  void mousePressEvent(QMouseEvent *) override;
  void paintEvent(QPaintEvent *) override;
  void rebuild();
  int currentPlayer() const;
signals:
  void playerChanged(int id);
  void newPlayer();
  void gearPressed();
private:
  class QSvgRenderer *settings_icon;
  QString last_;
};

#endif
