// Probability.C

/* Probability is a class for normalizing probabilities.
   Currently it can manage only integers, but a more proper impl'n would
   use templates so that any data type can be managed.
 * Operation: use add() to add a result to the list of possible results.
   The first arg is the unrenormalized probability for this result.
 * Calling operator() with a number between 0 and 1 will return one of the
   results previously added, with probabilities proportional to the original
   quoted probabilities.
 */

#include <string>
#include "../basics/Throw.h"
#include "../basics/dbx.h"
#include "Probability.h"
#include "../basics/getline.h"
#include <stdio.h>
void Probability::add(float prob, int res) {
  data.push_back(ProbLevel(max += prob, res));
}

istream &operator>>(istream &is, Probability &pr) {
  string s;
  dbx(2, "istream >> Probability");
  bool ok = false;
  while (s = getline_nocmt(is), s != "" && s[0] != '-') {
    double p;
    int r;
    sscanf(s.c_str(), "%i %lf", &r, &p);
    dbx(3, "P: addmany `%s' -> %i %g", s.c_str(), r, p);
    pr.add(p, r);
    ok = true;
  }
  skipcmt(is);
  if (!ok)
    athrow("istream >> Probability: nothing read");
  dbx(3, "istr >> Prob done");
  return is;
}

int Probability::operator()(float idx) const {
  idx *= max;
  for (vector<ProbLevel>::const_iterator i = data.begin();
       i != data.end();
       ++i)
    if (idx < (*i).thresh)
      return (*i).res;

  athrow("Probability::operator(): bad index");
  return -1; // never executed
}

istream &operator>>(istream &is, ProbBSet &pbs) {
  dbx(1, "istream >> ProbBSet");
  while (is.good()) {
    pbs.add(Probability());
    is >> pbs.v[pbs.number() - 1];
  }
  dbx(2, "istr >> ProbBS done");
  return is;
}

// static void __never_call_this() {
///* This fn exists solely to force the compiler to create
// vector<ProbLevel>::operator=(vector<ProbLevel> const &)
// */
// vector <ProbLevel> a, b;
// a=b;
// }