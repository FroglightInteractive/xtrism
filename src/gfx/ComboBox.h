// ComboBox.h

#ifndef COMBOBOX_H

#define COMBOBOX_H

#include "TextButton.h"

class ComboBox: public TextButton {
  Q_OBJECT;
public:
  ComboBox(QWidget *parent=0);
  ~ComboBox();
  bool selectItem(QString);
  void setItems(QStringList);
  void mousePressEvent(QMouseEvent *) override;
signals:
  void itemChanged(QString); // not emitted from selectItem
private:
  int idx_;
  QStringList items_;
};

#endif
