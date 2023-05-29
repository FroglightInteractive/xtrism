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
signals:
  void playerChanged(int id);
  void gearPressed();
private:
  class QSvgRenderer *settings_icon;
};

#endif
