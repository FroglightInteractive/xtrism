// BricksetSelector.cpp

#include "BricksetSelector.h"
#include "Options.h"

QStringList const &BricksetSelector::names() {
  static QStringList names{"Classic (7)",
    "Disjointed (17)",
    "Double disjointed (27)",
    "Totally unghinged (34)"};
  return names;
}

BricksetSelector::BricksetSelector(QWidget *parent): ComboBox(parent) {
  setItems(names());
  connect(this, &ComboBox::itemChanged,
          this, [this](QString s) {
            emit bricksetChanged(names().indexOf(s));
          });
}

BricksetSelector::~BricksetSelector() {
}

bool BricksetSelector::selectBrickset(int id) {
  return selectItem(names()[id]);
}
