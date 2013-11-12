// Ranker.H

#ifndef _Ranker_H
#define _Ranker_H

#include <string>
#include <vector>
using namespace std;

class Ranker {
  public:
    Ranker(/* class ScorerID const &sid, class Guinness const *book */);
    const string &operator()(int score);
    bool newrank(int score);
  private:
    string const *current;
    int nextrank;
    vector<int> scores;
    vector<string> labels;
  };

#endif
