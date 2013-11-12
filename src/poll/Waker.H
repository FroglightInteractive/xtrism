// Waker.H

#ifndef _Waker_H
#define _Waker_H

#include <list>

/* Suggestion: those classes that use sendwarn()'s code are _strongly_
   encouraged to contain a public anonymous enum containing labels for
   their codes, named W_NONE=0, W_XXX=1, etc. */

class Waker {
  public:
    Waker() {}
    virtual ~Waker();
    void reg(class Sleeper *sleeper);
    void dereg(class Sleeper const *sleeper);
  protected:
    void sendwarn(int code=0);
  std::list<class Sleeper *> sleepers;
  };

#endif
