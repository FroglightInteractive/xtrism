// Referee.H

#ifndef _Referee_H
#define _Referee_H

class Referee {
  public:
    Referee(class Kronos &kr0): kr(kr0), prepd(false) {}
    void brickprepare(int y_drop, bool selfdrop);
    double brickscore(int y_land);
    double linesscore(int lines);
  private:
    class Kronos &kr;
    int dt_godown, dt_used, y_drop;
    bool prepd, selfdrop;
  };

#endif
