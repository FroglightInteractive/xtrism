// MarbleBG.H

#ifndef _MarbleBG_H
#define _MarbleBG_H

void marblebg(unsigned int w, unsigned int h, unsigned int bw,
              float left, float top, float right, float bottom,
              class RGBMap &img, int offset_x=0, int offset_y=0,
              class ByteMap const *bytemap=0);
/* Renders a WxH area of marbled background at offset (OFFSET_X, OFFSET_Y)
   on the given IMG, using a border width BW, and assuming a relative
   location on the “rainbow” landscape given by LEFT, TOP, RIGHT, BOTTOM.
   (Relative coordinates run from 0 to 1.) Optionally, BYTEMAP specifies
   a pre-rendered source for rendering. If not given, we generate a
   BRICKCELL on the spot.
*/

#endif
