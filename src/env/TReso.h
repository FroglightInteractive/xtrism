// TReso.H -- dev.indep

#ifndef _TReso_H
#define _TReso_H

class TReso { // dev.indep.
public:
  TReso(int x, int y, int minfact, int maxfact, bool fs=0):
    x(x), y(y), minfact(minfact), maxfact(maxfact), fullscr(fs) {
  }
  int best(int sw, int sh) const;
  int actx(int f)  const {
    return x * f;
  }
  int acty(int f) const {
    return y * f;
  }
  bool fullscreen() const {
    return fullscr;
  }
private:
  int x, y, minfact, maxfact;
  bool fullscr;
};

#endif
