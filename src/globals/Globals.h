// Globals.H

#include "../options/PlayerList.h"
#include <QString>

class MainWindow *mainwindow();
class SBrickData const &sbd();
class BrickSprites const &bs();
class BrickSprites const &bs2();
class Sounds &sound();
class SPlayer &splayer();

QString datadir();
QString cachedir();

class GlobalOpts &globalopts();
class PlayerList &players();

int brickset();
void setbrickset(int);
PlayerList::PlayerIt player(bool right);
void setplayer(bool right, PlayerList::PlayerIt player);

void global_init(int argc, char **argv, class QApplication *);
void global_destroy();
