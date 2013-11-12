#include <stdio.h>

void f() throw (char*) {
  throw "hello";
  }

void main() {
  try
    { printf("1\n");
      f();
      printf("2\n");
    }
  catch (char *x)
    { printf("catch: %s\n",x);
    }
  catch (...)
    { printf("catch: unknown\n");
    }
  printf("3\n");
  }
