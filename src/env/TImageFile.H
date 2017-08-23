// TImagefile.H - a fileformat for TImages

#ifndef _TImagefile_H
#define _TImagefile_H

class TImageFile {
public:
  TImageFile(const class TEnv &tenv, const class Filename &basename);
  ~TImageFile();
  class TImage *read(class TImage *im = 0);
  void write(class TImage const *im);
  bool readable() const {
    return readfile != 0;
  }
public:
  struct Props {
    unsigned short width, height;
    unsigned short linelen;
  };
private:
  class TCompress *writefile;
  class TDecompress *readfile;
  const class TEnv &env;
};

#endif