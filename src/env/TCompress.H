// TCompress.H

// Code for more or less transparent reading and writing of compressed files

#ifndef _TCompress_H
#define _TCompress_H

#include "../basics/Filename.H"
#include "../basics/Infty.H"
#include "../basics/Byte.H"
#include <stdio.h>

class TCompress {
  public:
    TCompress(Filename const &fn);
    ~TCompress();
    int write(void const *adr, unsigned int len);
    // returns # of bytes NOT written - =0 unless error
  private:
    FILE *pipe;
    int cpid;
  };

const int TDCHUNK=64;

class TDecompress {
  public:
    TDecompress(Filename const &fn);
    ~TDecompress();
    int read(void *adr, unsigned int len);
    // returns # of bytes NOT read - =0 unless error or eof
    int readln(char *dst);
    // returns # of bytes written to buffer, excl. term. zero.
    // if zero, this can be an empty line, or eof, so DO check.
    bool eof(); // after read() returns non-zero, use this to check if OK.
  private:
    FILE *pipe;
    int cpid;
    byte buffer[TDCHUNK];
    unsigned int usedbuflen;
    unsigned int bufstartoff;
  };
#endif
