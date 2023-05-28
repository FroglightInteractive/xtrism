// MenuBox.h

#ifndef MENUBOX_H

#define MENUBOX_H

#include "MarbledButton.h"

class MenuBox: public MarbledButton {
  Q_OBJECT;
public:
  MenuBox(QWidget *parent=0);
  ~MenuBox();
  void setItems(QStringList const &);
  bool selectItem(QString); // true if successful, i.e., in list
  int heightForLineCount(int) const;
  void paintEvent(QPaintEvent *) override;
  void keyPressEvent(QKeyEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
  void mouseReleaseEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  QString exec(); // local event loop. Return null if no selection.
private:
  bool needsArrows() const;
  int lineHeight() const;
  int topMargin() const;
  int bottomMargin() const;
  int arrowHeight() const;
  void drawUpArrow(class QPainter *);
  void drawDownArrow(class QPainter *);
  void drawItem(class QPainter *, int idx, int y);
  void scroll(int dy, bool pg); // -ve for up arrow, +ve for down arrow
  int itemAt(int y) const; // <0 for up arrow, >=len(items_) for down arrow
signals:
  void itemClicked(QString);
  void cancel(); // private, for exec
private:
  int top_; // index of topmost item, if scrolling
  int idx_; // or -1
  int onarrow_; // -1 for up, +1 for down, 0 for neither
  QStringList items_;
  class QTimer *scrollTimer_;
};

#endif
