// PlayerSelector.cpp

#include "PlayerSelector.h"
#include "Options.h"

PlayerSelector::PlayerSelector(QWidget *parent): ComboBox(parent) {
  QStringList names;
  Options const &options(Options::instance());
  for (int id: options.allPlayerIDs())
    names << options.player(id).name();
  setItems(names);
  connect(this, &ComboBox::itemChanged,
          this, [this](QString s) {
            emit playerChanged(Options::instance().findPlayer(s));
          });
}

PlayerSelector::~PlayerSelector() {
}

bool PlayerSelector::selectPlayer(int id) {
  return selectItem(Options::instance().player(id).name());
}
