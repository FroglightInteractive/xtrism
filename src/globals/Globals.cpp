// Globals.C

#include <string>

#include "Globals.h"
#include "../env/TEnv.h"
#include "../env/TFont.h"
#include "../bricks/bsprites.h"
#include "../basics/dbx.h"
#include "../options/GlobalOpts.h"
#include "../options/PlayerList.h"
#include "../bricks/Probability.h"
#include "../bricks/data.h"
#include "../basics/Infty.h"
#include "../basics/Filename.h"
#include "../sound/Sounds.h"
#include "../env/TReso.h"

#include "../.datadir"

#include <fstream>

TEnv *gl_tenvp = 0;
SBrickData *gl_sbdp = 0;
BrickSprites *gl_bsp = 0;
BrickSprites *gl_bsp2 = 0;
TMFont *gl_tmfp = 0;
TFont *gl_tfp = 0;
TFont *gl_tfyp = 0;
Sounds *gl_soundsp = 0;

Filename *gl_datadirp = 0;
Filename *gl_cachedirp = 0;

GlobalOpts *gl_glopts = 0;
PlayerList *gl_pll = 0;
ProbBSet *gl_probbs = 0;

int gl_bset = 0;
PlayerList::PlayerIt gl_player[2];

static int gl_inited = false;

#include <string.h>
#include <stdio.h>

void global_init(int argc, char **argv) {
  int minf = 20;
  int maxf = 300;
  if (argc > 2 && strcmp(argv[1], "-size") == 0)
    minf = maxf = atoi(argv[2]);
  dbx(1, "global_init()");
  if (gl_inited)
    athrow(string("global_init(): double initialization"));
  gl_inited = true;
  gl_tenvp = new TEnv(TReso(16, 12, minf, maxf, true), argc, argv, ":0.0");
  dbx(1, "Actual factor: %i\n", gl_tenvp->actualfactor());

  char fontpattern[] = "-*-bitstream charter-bold-r-*-*-%i-*-*-*-*-*-*-*";
  int fontsizes[] = { 8, 10, 12, 14, 18, 24, 36, 48, 72, 96, INFTY };
  int maxsize = int(gl_tenvp->actualfactor() * .32);
  int idx = 0;
  while (fontsizes[idx + 1] <= maxsize)
    idx++;
  char buf[80];
  sprintf(buf, fontpattern, fontsizes[idx]);
  gl_tmfp = new TMFont(*gl_tenvp, buf);
  gl_tfp = new TFont(*gl_tmfp, TRGB(255), TRGB(0));
  gl_tfyp = new TFont(*gl_tmfp, TRGB(255, 255, 0), TRGB(0, 0, 0));

  gl_datadirp = new Filename(DATADIR);
  gl_cachedirp = new Filename(datadir() + "cache");
  gl_soundsp = new Sounds(datadir() + "sound");
  gl_tenvp->reg_splayer(gl_soundsp->player());

  dbx(2, "Reading bricks: data");
  gl_sbdp = new SBrickData(datadir() + "bricks");
  dbx(2, "Creating bricks: sprites");
  gl_bsp = new BrickSprites(*gl_sbdp, cachedir() + "bs",
                            int(gl_tenvp->actualfactor() / 2.5),
                            *gl_tenvp);
  gl_bsp2 = new BrickSprites(*gl_sbdp, cachedir() + "bs2",
                             int(gl_tenvp->actualfactor() / 2.5),
                             *gl_tenvp, 1);

  {
    dbx(2, "Creating probabilities");
    gl_probbs = new ProbBSet();
    ifstream ff((datadir() + "probs").name(),
                istream::in /* | ios::nocreate */);
    if (!ff)
      athrow("Couldn't open Probability file");
    ff >> *gl_probbs;
  }
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

TFont &tf() {
  dbx(3, "tf()");
  if (!gl_tfp)
    athrow(string("No TFont available"));
  return *gl_tfp;
}

TFont &tfyellow() {
  if (!gl_tfyp)
    athrow(string("No TFont available"));
  return *gl_tfyp;
}

Filename &datadir() {
  if (!gl_datadirp)
    athrow("No datadir available");
  return *gl_datadirp;
}

Filename &cachedir() {
  if (!gl_cachedirp)
    athrow("No cachedir available");
  return *gl_cachedirp;
}

ProbBSet &probbset() {
  if (!gl_probbs)
    athrow("No ProbBSet available");
  return *gl_probbs;
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