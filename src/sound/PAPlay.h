// PAPlay.h

#ifndef PAPLAY_H

#define PAPLAY_H

#include <QThread>
#include "Stereo.h"
#include <QMutex>
#include <pulse/pulseaudio.h>

class PAPlay: protected QThread {
public:
  virtual Stereo const *provide(int nscans);
public:
  PAPlay(QString name);
  ~PAPlay();
  void start();
  void stop();
  void setRate_Hz(int f);
  void setLatency_ms(int t);
  void setProcessTime_ms(int t);
protected:
  void run() override;
private:
  static void _context_state_callback(pa_context *c, void *userdata);
  static void _stream_write_callback(pa_stream *s, size_t len, void *userdata);
  static void _stream_drain_complete(pa_stream *s, int success, void *userdata);
  static void _context_drain_complete(pa_context *s, void *userdata);
private:
  void quit(int ret);
  void start_drain();
  void stream_drain_complete(int success);
  void context_drain_complete();
  void context_state_callback();
  // void stream_state_callback();
  void stream_write_callback(size_t len);
  // void stream_underflow_callback();
  // void stream_started_callback();
  // void stream_suspended_callback();
  // void stream_moved_callback();
  // void stream_event_callback();
  // void stream_buffer_attr_callback();
private:
  uint32_t sample_rate_hz;
  uint32_t latency_ms;
  uint32_t process_time_ms;
private:
  QMutex mutex;
  QString myname;
  bool prestart;
private:
  pa_context *context;
  pa_stream *stream;
  pa_mainloop_api *mainloop_api;
  
};

#endif
