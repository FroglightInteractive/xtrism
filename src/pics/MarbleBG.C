// MarbleBG.C

#include "../env/TImage.H"
#include "../bytemap/ByteMap.H"
#include "../bricks/brickcell.H"
#include "../env/TEnv.H"
#include "../basics/Throw.H"
#include "../basics/dbx.H"

#include <math.h>
#ifndef PI
#define PI 3.14159265358979323846               /* pi */
#endif

const float X0 = 0.5;
const float Y0 = 0.5;
const float DECAY = 2. / 3;
const float GREY0 = 0;
const float dGREY = 1.5;

void marblebg(unsigned int w, unsigned int h, unsigned int bw,
              float left, float top, float right, float bottom,
              TImage &img, int offset_x, int offset_y,
              ByteMap const *bytemap) {
  if (offset_x < 0 || offset_y < 0
      || offset_x + w > img.width() || offset_y + h > img.height())
    // return;
    athrow("marblebg: doesn't fit in TImage");
  BrickCell *bc = 0;
  if (!bytemap || bytemap->width() < w || bytemap->height() < h) {
    bc = new BrickCell(w, h);
    if (bw)
      bc->drawborders(bw, BCSurround());
    bytemap = bc;
  }

// now bytemap points to a proper size BrickCell. Let's go!

  float width = right - left;
  left -= X0;
  float height = bottom - top;
  top -= Y0;
  dbx(1, "marblebg: tl=(%g,%g) wh=(%g,%g)", left, top, width, height);
  dbx(1, "          offset=(%i,%i)", offset_x, offset_y);
  TCmap const &cm = img.tenv().cmap();
  for (unsigned int y = 0; y < h; y++)
    for (unsigned int x = 0; x < w; x++) {
      float phi = atan2(left + width * x / w, -top - height * y / h);
      phi -= PI / 6;
      if (phi < -PI) phi += 2 * PI;
      float green = max(cosf(phi * DECAY), 0.f);
      phi -= 2 * PI / 3;
      if (phi < -PI) phi += 2 * PI;
      float red = max(cosf(phi * DECAY), 0.f);
      phi -= 2 * PI / 3;
      if (phi < -PI) phi += 2 * PI;
      float blue = max(cosf(phi * DECAY), 0.f);
      float grey = bytemap->cc(x, y) * dGREY + GREY0;
      img.putpix(x + offset_x, y + offset_y,
                 cm(TRGB(grey * red, grey * green, grey * blue)));
    }

  if (bc)
    delete bc;
}