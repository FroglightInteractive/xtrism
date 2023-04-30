// MarbledButton.h

#ifndef MARBLEDBUTTON_H

#define MARBLEDBUTTON_H

#include <QWidget>

class MarbledButton: public QWidget {
public:
  MarbledButton(QString cachedir, QWidget *parent=0);
  virtual ~MarbledButton();
  void paintEvent(QPaintEvent *) override;
private:
  void generate();
private:
  QPixmap bg;
  QString cachedir;
  QPoint topleft;
};

#endif
