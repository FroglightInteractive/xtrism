// Filename.C

#include "Filename.H"
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

static string filename_dirname(const string &fn) {
  unsigned int n = fn.rfind(Fn_DirSep);
  if (n != 0 && n < fn.max_size())
    return string(fn, 0, n);
  else if (n == 0)
    return string("/");
  else
    return string(".");
}

static void filename_ensuredir(const string &dir) {
  struct stat s;
  // printf("ensuredir: dir=`%s'\n",dir.c_str());
  if (stat(dir.c_str(), &s) == 0 || dir == "/" || dir == ".")
    return;

  filename_ensuredir(filename_dirname(dir));
  // printf("Making directory `%s' (drwxr-x---)\n",dir.c_str());
  mkdir(dir.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP);
}

bool Filename::exist(bool cdir) const {
  // printf("exist `%s': cdir=%i\n",name(),int(cdir));
  struct stat s;
  bool res = stat(name(), &s) == 0;

  if (!res && cdir)
    filename_ensuredir(filename_dirname(nam));
  return res;
}