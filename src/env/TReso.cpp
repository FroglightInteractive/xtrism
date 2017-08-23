// TReso.C

#include "TReso.h"

#include "../basics/Throw.h"

int TReso::best(int sw, int sh) const {
  int f = minfact, af;
  int w = x * f, h = y * f;

// sw=sw<?640;

  tthrow(w > sw || h > sh, "TEnv: TReso: Display too small - cannot use it");
  do {
    af = f;
    f++;
    w += x;
    h += y;
  } while (f <= maxfact && w <= sw && h <= sh);
  return af;
}