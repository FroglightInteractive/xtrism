// Sample.cpp

#include "Sample.h"
#include <unistd.h>

Sample::Sample(QString fn) {
  cmd = "aplay ";
  cmd += fn;
  cmd += " &";
}

void Sample::play() {
  system(cmd.toLocal8Bit().data());
}
  
