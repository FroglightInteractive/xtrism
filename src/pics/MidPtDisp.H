// MidPtDisp.H

#ifndef _MidPtDisp_H
#define _MidPtDisp_H

class MidPtDisp {
public:
  MidPtDisp(unsigned int pow, unsigned int seed=1);
  ~MidPtDisp() {
    delete[] data;
  }
  float operator()(int x, int y) const {
    return (x < size && y < size) ? data[x + size * y] : 0;
  }
  int width() const {
    return size;
  }
  int height() const {
    return size;
  }
private:
  float &C(int x, int y) {
    return data[x + size * y];
  }
  float &SC(int x, int y) {
    return data[(x & sm) + size * (y & sm)];
  }
  int size;
  int logsize;
  int sm;
  float *data;
};

#endif