// MarbleBG.H

#ifndef _MarbleBG_H
#define _MarbleBG_H

void marblebg(unsigned int w, unsigned int h, unsigned int bw,
              float left, float top, float right, float bottom,
              class TImage &img, int offset_x=0, int offset_y=0,
              class ByteMap const *bytemap=0);

#endif
