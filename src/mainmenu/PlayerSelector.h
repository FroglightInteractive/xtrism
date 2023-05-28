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
signals:
  void playerChanged(int id);
};

#endif
