// HiscLine.H

#ifndef _HiscLine_H
#define _HiscLine_H

const int HLLEN=200;

// string format is: plrid date gamemode slev score lines blocks 

class HiscLine {
  public:
    HiscLine(char const *s);
    HiscLine(float score, int lines, int blocks, int slev);
    char const *read(char *s=0) const;
    bool operator<(HiscLine const &other) const { return score>other.score; }
    friend bool operator<(HiscLine const &a, HiscLine const &b);
    // WARNING: a HiscLine A is less than a HiscLine B if A's score is MORE
    // than B's score!!
    bool older(int date1) const { return date<date1; }
    bool worse(unsigned int score1) const { return score<score1; }
  private:
    unsigned int score;
    unsigned short lines;
    unsigned short blocks;
    unsigned short slev;
    int date;
  protected:
    static char buffer[HLLEN];
  };

inline bool operator<(HiscLine const &a, HiscLine const &b) {
  return a.score>b.score;
  }

class ATHLine: public HiscLine {
  public:
    ATHLine(char const *s);
    ATHLine(int plrid, float score, int lines, int blocks, int slev);
    char const *read(char *s=0) const;
    bool equalid(int id) const { return plrid==id; }
  private:
    int plrid;
  };

#endif
