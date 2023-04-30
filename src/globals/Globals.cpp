// Globals.C

#include <string>

#include "Globals.h"
#include "../env/MainWindow.h"
#include "../bricks/bsprites.h"
#include "../basics/dbx.h"
#include "../options/GlobalOpts.h"
#include "../options/PlayerList.h"
#include "../bricks/Probability.h"
#include "../bricks/data.h"
#include "../basics/Infty.h"
#include "../sound/Sounds.h"
#include "../env/TReso.h"

#include <fstream>

MainWindow *gl_mainwindowp = 0;
SBrickData *gl_sbdp = 0;
BrickSprites *gl_bsp = 0;
BrickSprites *gl_bsp2 = 0;
Sounds *gl_soundsp = 0;

QDir *gl_datadirp = 0;
QDir *gl_cachedirp = 0;

GlobalOpts *gl_glopts = 0;
PlayerList *gl_pll = 0;

int gl_bset = 0;
PlayerList::PlayerIt gl_player[2];

static int gl_inited = false;

#include <string.h>
#include <stdio.h>

void global_init(int argc, char **argv, class QApplication *app) {
  int minf = 20;
  int maxf = 300;
  //if (argc > 2 && strcmp(argv[1], "-size") == 0)
  //  minf = maxf = atoi(argv[2]);
  dbx(1, "global_init()");
  if (gl_inited)
    throw "global_init(): double initialization";
  gl_inited = true;

  QFileInfo me(argv[0]);
  gl_datadirp = new QDir(me.path() + "/../data");
  gl_cachedirp = new QDir(me.path() + "/../cache");

  gl_mainwindowp = new MainWindow(TReso(16, 12, minf, maxf, true),
                                  cachedir());
  dbx(1, "Actual factor: %i\n", mainwindow()->actualfactor());

  gl_soundsp = new Sounds(datadir() + "sound");

  dbx(2, "Reading bricks: data");
  gl_sbdp = new SBrickData(datadir() + "bricks");
  dbx(2, "Creating bricks: sprites");
  gl_bsp = new BrickSprites(*gl_sbdp, cachedir(),
                            int(mainwindow()->actualfactor() / 2.5), 0);
  gl_bsp2 = new BrickSprites(*gl_sbdp, cachedir(),
                             int(mainwindow()->actualfactor() / 2.5), 1);

  {
    dbx(2, "Reading global options");
    gl_glopts = new GlobalOpts();
    ifstream ff((datadir() + "glopts").name(), ios::in /* | ios::nocreate */);
    if (!ff)
      athrow("Cannot open global options");
    ff >> *gl_glopts;
  }
  {
    dbx(2, "Reading player list");
    gl_pll = new PlayerList();
    ifstream ff((datadir() + "players").name(),
                ios::in /* | ios::nocreate */);
    if (!ff)
      athrow("Cannot open player list");
    ff >> *gl_pll;
  }

  gl_player[0] = gl_pll->begin();
  gl_player[1] = gl_pll->next(gl_player[0], true);

  dbx(-20070110, "global_init() end");
}

void global_destroy() {
  if (!gl_inited)
    athrow(string("global_destroy(): not initialized"));
  gl_inited = false;
  delete gl_glopts;
  delete gl_pll;
  delete gl_probbs;
  delete gl_tfp;
  delete gl_tfyp;
  delete gl_tmfp;
  delete gl_bsp;
  delete gl_sbdp;
  gl_tenvp->dereg_splayer(gl_soundsp->player());
  delete gl_soundsp;
  delete gl_datadirp;
  delete gl_cachedirp;
  delete gl_tenvp;
  dbx(2, "global_destroy(): end");
}

TEnv &tenv() {
  dbx(3, "tenv()");
  if (!gl_tenvp)
    athrow(string("No TEnv available"));
  return *gl_tenvp;
}

TGFX &tgfx() {
  return tenv();
}

SBrickData const &sbd() {
  dbx(3, "sbd()");
  if (!gl_sbdp)
    athrow(string("No SBrickData available"));
  return *gl_sbdp;
}

BrickSprites const &bs() {
  dbx(3, "bs()");
  if (!gl_bsp)
    athrow(string("No BrickSprites available"));
  return *gl_bsp;
}

BrickSprites const &bs2() {
  dbx(3, "bs()");
  if (!gl_bsp2)
    athrow(string("No BrickSprites available"));
  return *gl_bsp2;
}

QString datadir() {
  if (!gl_datadirp)
    throw("No datadir available");
  return gl_datadirp->absolutePath();
}

QString cachedir() {
  if (!gl_cachedirp)
    athrow("No cachedir available");
  return gl_cachedirp->absolutePath();
}

GlobalOpts &globalopts() {
  if (!gl_glopts)
    athrow("No GlobalOpts available");
  return *gl_glopts;
}

PlayerList &players() {
  if (!gl_pll)
    athrow("No PlayerList available");
  return *gl_pll;
}

PlayerList::PlayerIt player(bool right) {
  return gl_player[right ? 1 : 0];
}

void setplayer(bool right, PlayerList::PlayerIt plr) {
  gl_player[right ? 1 : 0] = plr;
}

int brickset() {
  return gl_bset;
}

void setbrickset(int bset) {
  if (bset >= 0 && bset < 4)
    gl_bset = bset;
}

Sounds &sound() {
  if (!gl_soundsp)
    athrow("No Sounds available");
  return *gl_soundsp;
}
