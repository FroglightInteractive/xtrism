// BricksetSelector.h

#ifndef BRICKSETSELECTOR_H

#define BRICKSETSELECTOR_H

#include "ComboBox.h"

class BricksetSelector: public ComboBox {
  Q_OBJECT;
public:
  static QStringList const &names();
  BricksetSelector(QWidget *parent=0);
  ~BricksetSelector();
  bool selectBrickset(int id);
signals:
  void bricksetChanged(int id);
};

#endif
