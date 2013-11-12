// Sample.H (ESD vsn)

#ifndef Sample_H

#include "SampleExc.H"

#include <string>

typedef short SampleVal;

class Sample {
public:
  Sample(char const *fn) throw (SampleExc); 
  ~Sample();
  char const *name() const { return name_.c_str(); }
  unsigned int length() const { return length_; }
  unsigned int sfreq() const { return sfreq_; }
  int id() const { return id_; }
  static void set_esd_handle(int h) { esd_handle=h; }
private:
  unsigned int length_;
  unsigned int sfreq_;
  string name_;
  int id_;
  static int esd_handle;
};

#endif
