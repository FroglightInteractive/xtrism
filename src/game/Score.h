// Score.H

#ifndef _Score_H
#define _Score_H

class Score {
public:
  Score(): score(0), bricks(0) {
  }
  void operator+=(double s) {
    score += s;
    bricks++;
  }
  int pts() const {
    return int(score);
  }
  double ppb() const {
    return bricks ? score / bricks : 0;
  }
private:
  double score;
  int bricks;
};

#endif