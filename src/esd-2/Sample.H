// Sample.H

#ifndef _Sample_H
#define _Sample_H

#include <stdio.h>
#include "SampleExc.H"

typedef signed short SampleVal;

class Sample {
  public:
    Sample(char const *fn) throw (SampleExc);
    ~Sample() { if (data) delete [] data; }
    SampleVal operator[](unsigned int i) const { return data[i]; }
    unsigned int length() const { return length_; }
    unsigned int sfreq() const { return sfreq_; }
  private:
    SampleVal *data;
    unsigned int length_;
    unsigned int sfreq_;
  };

#endif
