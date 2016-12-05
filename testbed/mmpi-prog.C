// prog.C - main part of testbed: user editable
// This vsn draws the main menu background through MMPict.

// Define your globals here. Static please.

#include "main.H"

#include "../pics/MMPict.H"
#include "../pics/MidPtDisp.H"

MMPict *mmp;
MidPtDisp *tbc;
int lasty;

unsigned int THP::init() {
  // Initialise your globals now
  tbc=new MidPtDisp(9,23452345);
  mmp=0;
  lasty=0;
  return (8-testbed.tenv.cmap().avecoldep())>?0;
  }

void THP::tpoll() {
  if (!mmp)
    mmp=new MMPict(testbed.rgbmap.width(), testbed.rgbmap.height(),
                   tbc, &testbed.rgbmap,false);
  if (lasty<(int)testbed.rgbmap.height())
    { int newy=(lasty+64)<?testbed.rgbmap.height();
      mmp->drawit(lasty,newy);
      done(BBox(0,lasty, (int)testbed.rgbmap.width(), newy));
      lasty=newy;
    }
  }
