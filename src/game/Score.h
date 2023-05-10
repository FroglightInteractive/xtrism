// Score.H

#ifndef _Score_H
#define _Score_H

class Score {
public:
  Score(): pts(0), brks(0) {
  }
  void operator+=(double s) {
    pts += s;
    brks ++;
  }
  int score() const {
    return int(pts);
  }
  int bricks() const {
    return brks;
  }
  double ppb() const {
    return brks ? pts / brks : 0;
  }
private:
  double pts;
  int brks;
};

#endif
