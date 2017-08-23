// TCompress.C

#include "TCompress.H"

#include "../basics/Throw.H"
#include "../basics/dbx.H"
#include "../basics/minmax.H"

#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

const int COMPRESS_MODE = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; // -wr-r--r--

TCompress::TCompress(Filename const &fn) {
  fn.exist(true); // create the necessary directories
  int fd = open(fn.name(), O_WRONLY | O_CREAT, COMPRESS_MODE);
  tthrow(fd < 0, string("TCompress: failed to open `") + fn.name() + "': "
         + strerror(errno));
  int pfd[2];
  tthrow(::pipe(pfd), string("TCompress: pipe() failed: ") + strerror(errno));
  // pfd[0] is for reading (by gzip!)
  // pfd[1] is for writing (by me!)
  int fres = fork();
  if (fres == 0) { // child process
    close(pfd[1]);   // this is for parent
    close(0);
    dup(pfd[0]);             // stdin = pipe
    close(1);
    dup(fd);             // stdout = file
    execlp("gzip", "gzip", "-", (char *)NULL);
    fprintf(stderr, "TCompress: execlp returned\n");
    exit(1);
  }
  tthrow(fres < 0, string("TCompress: fork() failed: ") + strerror(errno));
  close(pfd[0]);
  close(fd);                // those are for child
  pipe = fdopen(pfd[1], "wb");
  cpid = fres;
}

TCompress::~TCompress() {
  dbx(1, "~TCompress");
  /*int r=*/ fclose(pipe);
  int stt;
  waitpid(cpid, &stt, 0);
  dbx(2, "~TC: wait returned");
  fthrow(WIFEXITED(stt), "~TCompress: child didn't exit!!?");
  tthrow(WEXITSTATUS(stt),
         "~TCompress: compression failed: child exited with non-zero code");
}

// static int totlen=0;

int TCompress::write(void const *adr, unsigned int len) {
  return len - fwrite(adr, 1, len, pipe);
}

TDecompress::TDecompress(Filename const &fn):
  usedbuflen(0) {
  int fd = open(fn.name(), O_RDONLY);
  tthrow(fd < 0, string("TDecompress: failed to open `") + fn.name() + "': "
         + strerror(errno));
  int pfd[2];
  tthrow(::pipe(pfd),
         string("TDecompress: pipe() failed: ") + strerror(errno));
  // pfd[0] is for reading (by me!)
  // pfd[1] is for writing (by gunzip!)
  int fres = fork();
  if (fres == 0) { // child process
    close(pfd[0]);   // is for parent
    close(0);
    dup(fd);             // stdin = file
    close(1);
    dup(pfd[1]);             // stdout = pipe
    execlp("gunzip", "gunzip", "-", (char *)NULL);
    fprintf(stderr, "TCompress: execlp returned\n");
    exit(1);
  }
  tthrow(fres < 0, string("TCompress: fork() failed: ") + strerror(errno));
  close(pfd[1]);
  close(fd);                // those are for child
  pipe = fdopen(pfd[0], "rb");
  cpid = fres;
}

TDecompress::~TDecompress() {
  fclose(pipe);
  int stt;
  waitpid(cpid, &stt, 0);
  fthrow(WIFEXITED(stt), "~TDecompress: child didn't exit!!?");
  tthrow(WEXITSTATUS(
           stt),
         "~TDecompress: decompression failed: child exited with non-zero code\nIf this was a file in .../cache/, delete it and try to rerun trism.");
}

int TDecompress::read(void *adr, unsigned int len) {
  if (usedbuflen) {
    unsigned int readlen = min(len, usedbuflen);
    memcpy(adr, buffer + bufstartoff, readlen);
    bufstartoff += readlen;
    usedbuflen -= readlen;
    adr = (void *)((byte *)adr + readlen);
    len -= readlen;
  }
  return len ? (len - fread(adr, 1, len, pipe)) : 0;
}

int TDecompress::readln(char *dst) {
  int totallen = 0;
  while (1) {
    if (!usedbuflen) {
      usedbuflen = TDCHUNK - read(buffer, TDCHUNK);
      bufstartoff = 0;
    }
    byte *nextnl
      = usedbuflen ? reinterpret_cast<byte *>(memchr(buffer + bufstartoff,
                                                     '\n',
                                                     usedbuflen))
        : buffer + bufstartoff;
    if (nextnl) {
      int thislen = nextnl - (buffer + bufstartoff);
      memcpy(dst, buffer + bufstartoff, thislen);
      totallen += thislen;
      dst[thislen] = 0;
      bufstartoff += thislen + 1;
      usedbuflen -= thislen + 1;
      return totallen;
    }
    memcpy(dst, buffer + bufstartoff, usedbuflen);
    dst += usedbuflen;
    totallen = usedbuflen;
  }
}

bool TDecompress::eof() {
  return feof(pipe);
}