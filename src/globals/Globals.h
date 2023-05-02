// Globals.H

#include <QString>

class MainWindow *mainwindow();
class SBrickData const &sbd();
class BrickSprites const &bs();
class BrickSprites const &bs2();
class Sounds &sound();
class SPlayer &splayer();

QString datadir();
QString cachedir();

class Options &options();

void global_init(int argc, char **argv, class QApplication *);
void global_destroy();
