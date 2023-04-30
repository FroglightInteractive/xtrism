// MMBG.C

#include "MMBG.h"
#include "MMPict.h"
#include "../env/TEnv.h"
#include "../bytemap/RGBImage.h"
#include <QFileInfo>
#include <QDebug>

MMBG::MMBG(TEnv const &env, QString filename):
  TImage(env, env.width(), env.height()) {
  if (QFileInfo(filename).exists()) {
    load(filename);
    return;
  }

  // we'll have to build it from scratch...
  qDebug() << "Making main menu background (this may take a while)\n";
  MMPict mmp(width(), height());
  *this = *mmp.rgbmap();
  
  if (!filename.isEmpty())
    write(filename);
}
