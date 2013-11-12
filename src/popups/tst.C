#include <string>
#include <stdio.h>
#include "Chooser.H"

int main(int argc, char *argv[]) {
  if (argc<2)
    return 1;
  Chooser chooser(0);
  chooser << CItem("Steven de Rooy") << CItem("Daniel Wagenaar");
  chooser << CItem("Steven de Neus") << CItem("Daniel Wauwel");

  Chooser::Iterator x(chooser.grep(argv[1]));
  while (x.good())
    { printf("%s matches %s\n",argv[1],x.text().c_str());
      ++x;
    }
  return 0;
  }
