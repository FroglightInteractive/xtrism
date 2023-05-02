// Globals.C


#include <QStandardPaths>
#include "Globals.h"
#include "../env/MainWindow.h"
#include "BrickSprites.h"
#include "../basics/dbx.h"
#include "../options/Options.h"
#include "../bricks/Probability.h"
#include "BrickData.h"
#include "../basics/Infty.h"
#include "../sound/Sounds.h"
#include "../env/TReso.h"

#include "brickdefs.h"


MainWindow *gl_mainwindowp = 0;
BrickSprites *gl_bsp = 0;
BrickSprites *gl_bsp2 = 0;
Sounds *gl_soundsp = 0;

int gl_bset = 0;

Options *gl_options = 0;

static int gl_inited = false;

#include <string.h>
#include <stdio.h>

void global_init(int argc, char **argv, class QApplication *app) {
  int minf = 20;
  int maxf = 300;
  //if (argc > 2 && strcmp(argv[1], "-size") == 0)
  //  minf = maxf = atoi(argv[2]);
  dbx(1, "global_init()");
  QDir(datadir()).mkpath(".");
  QDir(cachedir()).mkpath(".");
  if (gl_inited)
    throw "global_init(): double initialization";
  gl_inited = true;

  gl_mainwindowp = new MainWindow(TReso(16, 12, minf, maxf, true));
  qDebug() << "mw" << gl_mainwindowp->size();

  gl_soundsp = new Sounds(); //datadir() + "sound");

  dbx(2, "Creating bricks: sprites");
  gl_bsp = new BrickSprites(gl_sbd, cachedir(),
                            int(mainwindow()->actualfactor() / 2.5), 0);
  gl_bsp2 = new BrickSprites(gl_sbd, cachedir(),
                             int(mainwindow()->actualfactor() / 2.5), 1);

  gl_options = new Options();
}

void global_destroy() {
  if (!gl_inited)
    throw "global_destroy(): not initialized";
  gl_inited = false;
  delete gl_bsp;
  delete gl_soundsp;
  dbx(2, "global_destroy(): end");
}

SBrickData const &sbd() {
  return gl_sbd;
}

BrickSprites const &bs() {
  dbx(3, "bs()");
  if (!gl_bsp)
    throw "No BrickSprites available";
  return *gl_bsp;
}

BrickSprites const &bs2() {
  dbx(3, "bs()");
  if (!gl_bsp2)
    throw "No BrickSprites available";
  return *gl_bsp2;
}

QString datadir() {
  return QStandardPaths::writableLocation(
                       QStandardPaths::AppDataLocation);
}

QString cachedir() {
  return QStandardPaths::writableLocation(
                       QStandardPaths::CacheLocation);
}

Sounds &sound() {
  if (!gl_soundsp)
    throw "No Sounds available";
  return *gl_soundsp;
}

Options &options() {
  if (!gl_options)
    throw "No Options";
  return *gl_options;
}

MainWindow *mainwindow() {
  if (!gl_mainwindowp)
    throw "No main window";
  return gl_mainwindowp;
}
