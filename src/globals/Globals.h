// Globals.H

#include "../options/PlayerList.h"

class TEnv &tenv();
class TGFX &tgfx();
class SBrickData const &sbd();
class BrickSprites const &bs();
class BrickSprites const &bs2();
class TFont &tf();
class TFont &tfyellow();
class Sounds &sound();

class Filename &datadir();
class Filename &cachedir();

class GlobalOpts &globalopts();
class ProbBSet &probbset();
class PlayerList &players();

int brickset();
void setbrickset(int);
PlayerList::PlayerIt player(bool right);
void setplayer(bool right, PlayerList::PlayerIt player);

void global_init(int argc, char **argv);
void global_destroy();