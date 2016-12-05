// HiscList.H

#ifndef _HiscList_H
#define _HiscList_H

#include "HiscLine.H"
#include <multiset.h>

typedef multiset<HiscLine> HiscList_;
typedef multiset<ATHLine> ATHList_;

class HiscList {
  public:
    HiscList(int profile, int ath);
    HiscList(class TDecompress &src);
    ~HiscList();
    void save(class TCompress &dest) const;
    int insert(HiscLine const &hl);
    int insert(ATHLine const &hl);
    int profile() const { return profile_; }
    int ath() const { return ath_; } // 0=normal, +1=solo_ath, -1=team_ath
    void eraseolder(int date);
    void eraseworse(unsigned int score);
    void eraseid(int id);
    void keeponlybest(int num);
    HiscLine const &operator[](int n);
    int rank(unsigned int score, unsigned int *next=0) const;
  private:
    int profile_;
    int ath_;
    union {
        HiscList_ *hl;
        ATHList_ *athl;
      };
  };

#endif
