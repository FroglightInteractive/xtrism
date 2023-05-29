// TranspLabel.h

#ifndef TRANSPLABEL_H

#define TRANSPLABEL_H

#include <QWidget>

class TranspLabel: public QWidget {
  Q_OBJECT;
public:
  TranspLabel(QWidget *parent=0);
  TranspLabel(QString, QColor, Qt::Alignment, QWidget *parent=0);
  ~TranspLabel();
  void setText(QString);
  void setColor(QColor);
  void setAlignment(Qt::Alignment);
  void paintEvent(QPaintEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
  void keyPressEvent(QKeyEvent *) override;
  void enterEvent(QEnterEvent *) override;
  void focusInEvent(QFocusEvent *) override;
  void focusOutEvent(QFocusEvent *) override;
signals:
  void keyPressed(int code, QString txt, int qkey);
  void mousePressed();
private:
  QString text_;
  QColor color_;
  Qt::Alignment alignment_;
};


#endif
