// KeyEditor.cpp

#include "KeyEditor.h"
#include <QFileInfo>
#include "Rainbow.h"
#include "TranspLabel.h"
#include "MapIterator.h"
#include "Paths.h"
#include <QPainter>
#include <QEventLoop>
#include "MetaKeys.h"
#include "NativeKeys.h"
#include "Options.h"

KeyEditor::KeyEditor(Player const &p, QWidget *parent):
  QWidget(parent), plr(p) {
  resize(parent->size());
  move(0, 0);
  loadBackground();
  makeItems();
}

void KeyEditor::loadBackground() {
  QString fn = QString("%1/keyeditor-%3x%4.jpg")
    .arg(Paths::cachedir())
    .arg(width()).arg(height());
  if (QFileInfo(fn).exists()) {
    bg = QPixmap(fn);
  } else {
    QImage img = Rainbow(size(),
                         QRect(width()/3, height()*2/3, width()/2, height()/3),
                         size()).create().toQImage();
    img.save(fn);
    bg = QPixmap::fromImage(img);
  }
}

void KeyEditor::makeItems() {
  QColor yellow("#ffff00");
  QColor white("#ffffff");
  int w = int(.09*width());
  int h = int(.035*height());
  float dy = h;
  int x0 = .3*width();
  QMap<Sides::Side, int> xx{
    {Sides::Side::Solo, int(.42*width())},
    {Sides::Side::Left, int(.52*width())},
    {Sides::Side::Right, int(.62*width())}
  };
  int x1 = .52*width();
  float y = 2*dy;
  auto *lbl = new TranspLabel("EDITABLE MOVEMENT KEYS", yellow,
                              Qt::AlignCenter, this);
  lbl->setGeometry(QRect(0, y, width(), h));
  y += 1.4 * dy;
  
  lbl = new TranspLabel("When playing...", white, Qt::AlignCenter, this);
  lbl->setGeometry(QRect(x1, y, w, h));
  y += 1. * dy;

  for (auto it: MapRange(Sides::names())) {
    lbl = new TranspLabel(it.value(), yellow, Qt::AlignCenter, this);
    lbl->setGeometry(xx[it.key()], y, w, h);
  }
  y += 1.4 * dy;

  QMap<Sides::Side, TranspLabel *> last;
  for (auto kit: MapRange(GameKeys::names())) {
    GameKeys::Key k = kit.key();
    lbl = new TranspLabel(kit.value(), yellow,
                          Qt::AlignLeft | Qt::AlignVCenter, this);
    lbl->setGeometry(x0, y, w, h);
    for (auto sit: MapRange(xx)) {
      Sides::Side s = sit.key();
      lbl = new TranspLabel(this);
      lbl->setText(NativeKeys::name(plr.keys(s)[k]));
      lbl->setGeometry(xx[s], y, w, h);
      connect(lbl, &TranspLabel::keyPressed,
              this, [this, s, k, lbl](int scan) {
                plr.setKey(s, k, scan);
                lbl->setText(NativeKeys::name(scan));
                Options::instance().updatePlayer(plr);
                Options::instance().save();
                if (taborder.contains(lbl))
                  taborder[lbl]->setFocus();
                else
                  setFocus(); // take focus away from list
              });
      if (last[s])
        taborder[last[s]] = lbl;
      last[s] = lbl;
    }
    y += dy;
  }

  y += 2 * dy;
  
  lbl = new TranspLabel("OTHER KEYS", yellow, Qt::AlignCenter, this);
  lbl->setGeometry(QRect(0, y, width(), h));
  y += 1.4 * dy;
  
  lbl = new TranspLabel("When playing...", white, Qt::AlignCenter, this);
  lbl->setGeometry(QRect(x1, y, w, h));
  y += 1. * dy;
  
  for (auto it: MapRange(Sides::names())) {
    lbl = new TranspLabel(it.value(), yellow, Qt::AlignCenter, this);
    lbl->setGeometry(xx[it.key()], y, w, h);
  }
  y += 1.4 * dy;
  
  for (auto kit: MapRange(MetaKeys::names())) {
    lbl = new TranspLabel(kit.value(), yellow,
                          Qt::AlignLeft | Qt::AlignVCenter, this);
    lbl->setGeometry(x0, y, w, h);
    for (auto sit: MapRange(xx)) {
      lbl = new TranspLabel(this);
      lbl->setText(NativeKeys::name(Options::instance()
                                    .metakeys(sit.key())[kit.key()]));
      lbl->setGeometry(xx[sit.key()], y, w, h);
    }
    y += dy;
  }

  y += 2 * dy;

  lbl = new TranspLabel("Done", yellow, Qt::AlignCenter, this);
  connect(lbl, &TranspLabel::mousePressed,
          this, [this]() { done(); });
  lbl->setGeometry(QRect(.85*width(), y, w, h));
}
 
KeyEditor::~KeyEditor() {
}

void KeyEditor::paintEvent(QPaintEvent *) {
  QPainter p(this);
  p.drawPixmap(0, 0, bg);
}

void KeyEditor::exec() {
  QEventLoop el;
  show();
  connect(this, &KeyEditor::done, &el, &QEventLoop::quit);
  el.exec();
}
