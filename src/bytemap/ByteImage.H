// ByteImage.H

#ifndef _ByteImage_H
#define _ByteImage_H

/* byteimage copies a ByteMap into a TImage, using rgb0 and rgb1 as
   a linear map:

                 v |-> rgb0 + (rgb1-rgb0)*(v/255)

 * Note that rgb0 and rgb1 do not have to lie within [0,255]^3: the
   rgb values written into the TImage are clipped automatically.
 */
extern void byteimage(class TImage &dst, class ByteMap const &src,
                          class WideRGB const &rgb0,
                            class WideRGB const &rgb1);

#endif