// StatBoard.C

#include "StatBoard.h"
#include "RGBRecolor.h"
#include <QFontMetrics>
#include "BBox.h"
#include <QPainter>
#include <QDebug>

StatBoard::StatBoard(int nlines, int labelw, int dataw,
                     Rainbow const &sbg, QWidget *parent):
  QWidget(parent), sharedbg(sbg) {
  QFontMetrics fm(font());
  dy = fm.height();
  y0 = dy/3; // + fm.ascent();
  dxl = dy/2;
  dxd = dy*6/9;
  int wid = labelw + dataw + 3*dy;
  int hei = nlines*dy + dy;
  resize(wid, hei);
}

void StatBoard::setlabel(int i, QString const &txt, bool upd) {
  ensure(i);
  labels[i] = txt;
  if (upd)
    update();
}

void StatBoard::setdata(int i, QString const &txt, bool upd) {
  ensure(i);
  data[i] = txt;
  if (upd)
    update();
}

void StatBoard::setdata(int i, int val, bool update) {
  setdata(i, QString::number(val));
}

void StatBoard::setdata(int i, double val, bool update) {
  setdata(i, QString::number(val, 'f', 1));
}

void StatBoard::generate() {
  qDebug() << "StatBoard::generate" << x() << y() << width() << height();
  Rainbow clipped = sharedbg.cropped(geometry());
  int bw = 1 + width() / 75 + 1;
  BBox bb(bw, bw, width() - 2*bw, height() - 2*bw);
  clipped.image().recolorRect(bb, .75, 0);
  clipped.image().recolorRectEdges(bb, bw, -64);
  mybg = QPixmap::fromImage(clipped.create().toQImage());
  topleft = pos();
}

void StatBoard::paintEvent(QPaintEvent *) {
  QPainter p(this);
  if (pos()!=topleft || mybg.size()!=size())
    generate();
  p.drawPixmap(QPoint(0,0), mybg);

  p.setPen(QPen(QColor(255,255,255)));
  int w = width();
  for (int i=0; i<labels.size(); i++)
    p.drawText(QRectF(QPoint(dxl, y0 + dy*i), QSize(w - dxl, dy)),
               Qt::AlignLeft | Qt::AlignVCenter, labels[i]);
  p.setPen(QPen(QColor(255,255,0)));
  for (int i=0; i<labels.size(); i++)
    p.drawText(QRectF(QPoint(0, y0 + dy*i), QSize(w - dxd,dy)),
               Qt::AlignRight | Qt::AlignVCenter, data[i]);
}

void StatBoard::cleardata() {
  for (int i = 0; i < data.size(); i++)
    setdata(i, "");
}

void StatBoard::ensure(int i) {
  while (labels.size() <= i)
    labels.push_back("");
  while (data.size() <= i)
    data.push_back("");
}
