// MMBG.H - the image for the main menu

#ifndef _MMBG_H
#define _MMBG_H

#include "../env/TImage.h"

class MMBG: public TImage {
public:
  MMBG(class TEnv const &env, QString filename=QString());
};

#endif
