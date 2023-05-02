// TextButton.h

#ifndef TEXTBUTTON_H

#define TEXTBUTTON_H

#include "MarbledButton.h"

class TextButton: public MarbledButton {
public:
  TextButton(QWidget *parent=0);
  virtual ~TextButton();
  void paintEvent(QPaintEvent *) override;
  void setText(QString);
private:
  QString txt;
};

#endif
