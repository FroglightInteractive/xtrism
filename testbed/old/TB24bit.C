// TB24bit.C

#include "TB24bit.H"
#include "../env/TImage.H"
#include "../basics/BBox.H"
#include "../basics/Throw.H"

TB24bit::TB24bit(unsigned int w, unsigned int h): wid(w), hei(h) {
  data=new byte[3*w*h];
  for (unsigned int d=0; d<w*h*3; d++)
    data[d]=0;
  }

TB24bit::~TB24bit() {
  delete [] data;
  }


/* --------------------------------- fill --------------------------------- */
/* Uses TImage putpix directly. 
 * It would obviously be better for some images to do some dithering,
   esp. on low depth visuals.
 * The slowness of TImage.putpix is quite extreme, unfortunately. A quicker
   depth conversion routine would be quite nice...
 */
void TB24bit::fill(TImage &tim, BBox const &bb) {
  if (bb.top()<0 || bb.bottom()>hei ||
      bb.left()<0 || bb.right()>wid)
    { dbx(1,"TB24bit::fill: bad bbox: %i %i %i %i",
          bb.left(),bb.top(),bb.right(),bb.bottom());
      athrow("TB24bit::fill: bad bbox");
    }
  for (unsigned int y=bb.top(); y<bb.bottom(); y++)
    for (unsigned int x=bb.left(); x<bb.right(); x++)
      tim.putpix(x,y,tim.tenv().rgb(TRGB(r(x,y),g(x,y),b(x,y))));
  }
