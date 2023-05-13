// Score.H

#ifndef _Score_H
#define _Score_H

class Score {
public:
  Score() {
    reset();
  }
  void reset() {
    pts = 0;
    brks = 0;
    lns = 0;
  }
  void addScore(double s) {
    pts += s;
    brks ++;
  }
  void addLine(int dl=1) {
    lns += dl;
  }
  int score() const {
    return int(pts);
  }
  int bricks() const {
    return brks;
  }
  int lines() const {
    return lns;
  }
  double ppb() const {
    return brks ? pts / brks : 0;
  }
private:
  double pts;
  int brks;
  int lns;
};

#endif
