// TImagefile.C

#define TPRIVATE

#include "TImageFile.h"
#include "TImage.h"
#include "TEnv.h"
#include "TCompress.h"
#include "../basics/Filename.h"

static Filename fullname(TEnv const &tenv, Filename const &basename) {
  Filename fn(basename);
  fn.addpart(tenv.cmap().id());
  fn.addsuffix("timg");
  return fn;
}

TImageFile::TImageFile(const TEnv &tenv, const Filename &basename):
  writefile(0), readfile(0), env(tenv) {
  Filename fn(fullname(tenv, basename));
  if (fn.exist())
    readfile = new TDecompress(fn);
  else
    writefile = new TCompress(fn);
}

TImage *TImageFile::read(TImage *im) {
  fthrow(readfile, "TImageFile::read - not opened for reading");
  Props p;
  tthrow(readfile->read(&p, sizeof(Props)),
         "TImageFile::read - error reading props");
  if (im)
    tthrow(im->width() != p.width || im->height() != p.height
           || im->linelength() != p.linelen,
           "TImageFile::read: Trying to refill image with bad size");
  else
    im = new TImage(env, p.width, p.height);
  tthrow(readfile->read(im->data(), p.height * p.linelen),
         "TImageFile::read - error reading data");
  return im;
}

void TImageFile::write(TImage const *image) {
  fthrow(writefile, "TImageFile::write - not opened for writing");
  Props p;
  p.width = image->width();
  p.height = image->height();
  p.linelen = image->linelength();
  tthrow(writefile->write(&p, sizeof(Props)),
         "TImageFile::write: error writing props");
  tthrow(writefile->write(image->data(), p.height * p.linelen),
         "TImageFile::write: error writing data");
}

TImageFile::~TImageFile() {
  if (writefile)
    delete writefile;
  if (readfile)
    delete readfile;
}