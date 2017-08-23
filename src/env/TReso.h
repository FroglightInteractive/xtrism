// TReso.H -- dev.indep

#ifndef _TReso_H
#define _TReso_H

class TReso { // dev.indep.
public:
  TReso(int xx, int yy, int mminfact, int mmaxfact, bool fs=0):
    x(xx), y(yy), minfact(mminfact), maxfact(mmaxfact), fullscr(fs) {
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