// PlayerSelector.cpp

#include "PlayerSelector.h"
#include "Options.h"
#include <QPainter>
#include <QMouseEvent>
#include <QSvgRenderer>

PlayerSelector::PlayerSelector(QWidget *parent): ComboBox(parent) {
  rebuild();
}

void PlayerSelector::rebuild() {
  QStringList names;
  Options const &options(Options::instance());
  for (int id: options.allPlayerIDs())
    names << options.player(id).name();
  QString newplayer = "〈New Player〉";
  names << newplayer;
  setItems(names);
  last_ = currentItem();
  connect(this, &ComboBox::itemChanged,
          this, [this, newplayer](QString s) {
            if (s==newplayer) {
              selectItem(last_);
              emit newPlayer();
            } else {
              last_ = s;
              emit playerChanged(Options::instance().findPlayer(s));
            }
          });
  settings_icon = new QSvgRenderer(QString(":/settings.svg"), this);
}

PlayerSelector::~PlayerSelector() {
  delete settings_icon;
}

bool PlayerSelector::selectPlayer(int id) {
  return selectItem(Options::instance().player(id).name());
}

void PlayerSelector::paintEvent(QPaintEvent *e) {
  ComboBox::paintEvent(e);
  QPainter p(this);
  int h = height();
  float h0 = h/4.;
  settings_icon->render(&p, QRect(width()-3*h0, h0, 2*h0, 2*h0));
  //  p.setPen(QColor(255,255,255));
  //  p.drawText(QRect(width()-height(), 0, height(), height()),
  //             Qt::AlignCenter, "⛭");
}

void PlayerSelector::mousePressEvent(QMouseEvent *e) {
  QPoint p = e->pos();
  if (p.x() >= width() - height())
    emit gearPressed();
  else
    ComboBox::mousePressEvent(e);
}

int PlayerSelector::currentPlayer() const {
  return Options::instance().findPlayer(currentItem());
}
