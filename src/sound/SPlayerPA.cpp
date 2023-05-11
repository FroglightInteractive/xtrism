// SPlayerPA.cpp

#include "SPlayerPA.h"
#include <QDebug>
#include <chrono>
#include <thread>

SPlayerPA::SPlayerPA(): PAPlay("Trism") {
  setRate_Hz(44100);
  setLatency_ms(20);
  start();
}

SPlayerPA::~SPlayerPA() {
  stop();
  while (isRunning()) {
    qDebug() << "SPlayerPA waiting for thread to stop";
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }
}


Stereo const *SPlayerPA::provide(int nscans) {
  if (buffer.size() < nscans)
    buffer.resize(nscans);
  render(buffer.data(), nscans);
  return buffer.data();
}
