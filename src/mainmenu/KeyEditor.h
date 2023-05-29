// KeyEditor.h

#ifndef KEYEDITOR_H

#define KEYEDITOR_H

#include <QWidget>
#include "Player.h"

class KeyEditor: public QWidget {
  Q_OBJECT;
public:
  KeyEditor(Player const &p, QWidget *parent=0);
  ~KeyEditor();
  void paintEvent(QPaintEvent *) override;
  void exec();
private:
  void loadBackground();
  void makeItems();
signals:
  void done();
private:
  Player plr;
  QPixmap bg;
  QMap<class TranspLabel *, class TranspLabel *> taborder;
};

#endif
