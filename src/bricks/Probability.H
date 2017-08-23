// Probability.H

// The probabilities for each brick

#ifndef _Probability_H
#define _Probability_H

#include <vector>
#include <iostream>
using namespace std;

class ProbLevel {
public:
  ProbLevel(float t=0, int r=0): thresh(t), res(r) {
  }
protected:
  float thresh;
  int res;
  friend class Probability;
};

class Probability {
public:
  Probability(): max(0) {
  }
  void add(float prob, int res);
  int operator()(float idx) const;   // 0<=idx<1
private:
  float max;
  vector <ProbLevel> data;
};

istream &operator>>(istream &is, Probability &pr);

class ProbBSet {
public:
  ProbBSet() {
  }
  int number() {
    return v.size();
  }
  void add(Probability const &p) {
    v.push_back(p);
  }
  void add(int i, float prob, int res) {
    v[i].add(prob, res);
  }
  Probability const &operator[](int i) const {
    return v[i];
  }
protected:
  vector <Probability> v;
  friend istream &operator>>(istream &is, ProbBSet &pbs);
};

istream &operator>>(istream &is, ProbBSet &pbs);

#endif