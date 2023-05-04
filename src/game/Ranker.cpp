// Ranker.C

#include "Ranker.h"

// The highest high-score is stored first in the vector `scores'.

Ranker::Ranker(/* class ScorerID const &sid, class Guinness const *book */) {
  scores.push_back(100000000);
  labels.push_back("Impossible");

  scores.push_back(0);
  labels.push_back("Record");
  // Obviously, the last line should be replaced by a more ingenious
  // algorithm reading scores from the book.

  scores.push_back(0);
  labels.push_back("-");
  current = &labels[labels.size() - 1];
  nextrank = labels.size() - 2;
}

bool Ranker::newrank(int score) {
  if (score > scores[nextrank]) {
    current = &labels[nextrank--];
    return true;
  }
  return false;
}

const string &Ranker::operator()(int score) {
  newrank(score);
  return *current;
}
