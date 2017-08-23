// Infty.H

const int INFTY = 100000000; // 1e8

/* INFTY has been chosen such that if |x|<INFTY, then x+INFTY or x-INFTY
   do not cause overflow. In fact, in the current implementation,
   INFTY < MAXINT/20.
 */