#include <stdio.h>
class A {
  public:
    virtual ~A() { }
    virtual void foo()=0;
  };

class B: public A {
  public:
    B();
    virtual ~B();
  private:
    void foo();
    virtual void bla();
  };

B::B() {
  printf("B::B()\n");
  }
