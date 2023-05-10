// PAPlay.cpp

#include "PAPlay.h"

void PAPlay::_context_state_callback(pa_context *c, void *userdata) {
    PAPlay *paplay = (PAPlay*)(userdata);
    paplay->context_state_callback();
  }
  
void PAPlay::_stream_write_callback(pa_stream *s, size_t len, void *userdata) {
  PAPlay *paplay = (PAPlay*)(userdata);
  paplay->stream_write_callback(len);
}
  
void PAPlay::_stream_drain_complete(pa_stream *s, int success, void *userdata) {
  PAPlay *paplay = (PAPlay*)(userdata);
  paplay->stream_drain_complete(success);
}

void PAPlay::_context_drain_complete(pa_context *s, void *userdata) {
  PAPlay *paplay = (PAPlay*)(userdata);
  paplay->context_drain_complete();
}



void PAPlay::stream_write_callback(size_t len) {
  Scan *data = fill(len/4);
  if (data) {
    pa_stream_write(stream, (char*)(data), len, NULL, 0, PA_SEEK_RELATIVE);
  } else {
    pa_stream_cancel_write(stream);
    _start_drain();
  }
}

void PAPlay::start_drain() {
  if (stream) {
    pa_stream_set_write_callback(stream, NULL, NULL);
    pa_operation *o = pa_stream_drain(stream, _stream_drain_complete, this);
    if (!o) {
      qDebug() << "pa_stream_drain failed"
               << pa_strerror(pa_context_errno(context));
      quit(1);
    } else {
      pa_operation_unref(o);
    }
  } else {
    quit(0);
  }
}

void PAPlay::stream_drain_complete(int success) {
  if (!success) {
    qDebug() << "failed to drain stream"
             << pa_strerror(pa_context_errno(context));;
    quit(1);
  }
  pa_stream_disconnect(stream);
  pa_stream_unref(stream);
  stream = NULL;

  pa_operation *o = pa_context_drain(context, _context_drain_complete, this);
  if (!o) {
    pa_context_disconnect(context);
  } else {
    pa_operation_unref(o);
  }
}

void PAPlay::context_drain_complete() {
  pa_context_disconnect(context);
}

void PAPlay::context_state_callback() {
  switch (pa_context_get_state(context)) {
  case PA_CONTEXT_CONNECTING:
  case PA_CONTEXT_AUTHORIZING:
  case PA_CONTEXT_SETTING_NAME:
    break;
  case PA_CONTEXT_READY: {
    if (stream) {
      qDebug() << "stream already exists!?";
      goto fail;
    }

    stream = pa_stream_new(context, 0, &sample_spec, 0);
    if (!stream) {
      qDebug() << "pa_stream_new failed"
               << pa_strerror(pa_context_errno(context));
      goto fail;
    }

    //pa_stream_set_state_callback(stream, _stream_state_callback, this);
    pa_stream_set_write_callback(stream, _stream_write_callback, this);
    //pa_stream_set_suspended_callback(stream, _stream_suspended_callback, this);
    //pa_stream_set_moved_callback(stream, _stream_moved_callback, this);
    //pa_stream_set_underflow_callback(stream, _stream_underflow_callback, this);
    //pa_stream_set_started_callback(stream, _stream_started_callback, this);
    //pa_stream_set_event_callback(stream, _stream_event_callback, this);
    //pa_stream_set_buffer_attr_callback(stream, _stream_buffer_attr_callback, this);

    pa_buffer_attr buffer_attr;
    pa_zero(buffer_attr);
    buffer_attr.maxlength = (uint32_t)(-1);
    buffer_attr.prebuf = (uint32_t)(-1);
    pa_stream_flags_t flags = 0;
    pa_sample_spec sample_spec {
      .format = PA_SAMPLE_S16LE,
      .rate = sample_rate_hz,
      .channels = 2
    };

    if (pa_frame_size(&sample_spec) != 4) {
      qDebug() << "scan size is not 4 bytes";
      goto fail;
    }
    
    if (latency_msec > 0) {
      buffer_attr.fragsize
        = buffer_attr.tlength
        = pa_usec_to_bytes(latency_msec * PA_USEC_PER_MSEC, &sample_spec);
      flags |= PA_STREAM_ADJUST_LATENCY;
    } else {
      buffer_attr.fragsize = buffer_attr.tlength = (uint32_t)(-1);
    }

    if (process_time_msec > 0) 
      buffer_attr.minreq
        = pa_usec_to_bytes(process_time_ms * PA_USEC_PER_MSEC, &sample_spec);
    else
      buffer_attr.minreq = (uint32_t)(-1);

    if (pa_stream_connect_playback(stream, NULL, &buffer_attr, flags,
                                   NULL, NULL) < 0) {
      qDebug() << "pa_stream_connect_playback() failed"
               << pa_strerror(pa_context_errno(context));
      goto fail;
    }
  } break;
  case PA_CONTEXT_TERMINATED:
    quit(0);
    break;
  case PA_CONTEXT_FAILED:
    qDebug() << "connection failure"
             << pa_strerror(pa_context_errno(context));
    goto fail;
  }
  return;

 fail:
  quit(1);
}

PAPlay::PAPlay(QString name): myname(name) {
  sample_rate_hz = 44100;
  latency_ms = 0;
  process_time_ms = 0;
  
  context = 0;
  stream = 0;
  mainloop_api = 0;
  prestart = false;
}

void PAPlay::start() {
  mutex.lock();
  if (prestart || mainloop_api) {
    qDebug() << "already running";
  } else {
    prestart = true;
    exec();
  }
  mutex.unlock();
}

void PAPlay::stop() {
  mutex.lock();
  quit(0);
  mutex.unlock();
}

void PAPlay::setRate_Hz(int f) {
  sample_rate_hz = f;
}

void PAPlay::setLatency_ms(int t) {
  latency_ms = t;
}

void PAPlay::setProcessTime_ms(int t) {
  process_time_ms = t;
}

void PAPlay::run() {
  qDebug() << "paplay run";
  mutex.lock();
  qDebug() << "paplay run got mutex";
  prestart = false;
  pa_mainloop *m = pa_mainloop_new();
  if (!m) {
    qDebug() << "pa_mainloop_new failed";
    goto quit;
  }
  mainloop_api = pa_mainloop_get_api(m);
  if (!mainloop_api) {
    qDebug() << "pa_mainloop_get_api failed";
    goto quit;
  }
  context = pa_context_new(mainloop_api, myname.toUtf8().data());
  if (!context) {
    qDebug() << "pa_context new failed";
    goto quit;
  }
  pa_context_set_state_callback(context, _context_state_callback, this);

  if (pa_context_connect(context, 0, 0, NULL) < 0) {
    qDebug() << "pa_context_connect failed"
             << pa_strerror(pa_context_errno(context));
    goto quit;
  }

  qDebug() << "paplay enter loop";
  int ret = 0;
  mutex.unlock();
  if (pa_mainloop_run(m, &ret) < 0) {
    qDebug() << "mainloop_run failed";
  }
  mutex.lock();
  qDebug() << "paplay exit loop";

 quit:
  if (stream) {
    pa_stream_unref(stream);
    stream = 0;
  }
  if (context) {
    pa_context_unref(context);
    context = 0;
  }
  if (m) {
    pa_signal_done();
    pa_mainloop_free(m);
  }
  mainloop_api = 0;
  qDebug() << "paplay done";
}

void PAPlay::quit(int ret) {
  qDebug() << "quit" << ret;
  if (mainloop_api)
    mainloop_api->quit(mainloop_api, ret);
}

//////////////////////////////////////////////////////////////////////
PAPlay::Scan const *PAPlay::provide(int nscans) {
  qDebug() << "provide" << nscans;
  static QList<Scan> buffer;
  static int phase = 0;
  if (buffer.size() != nscans)
    buffer.resize(nscans);
  Scan *data = buffer.data();
  while (nscans>0) {
    data->left = 10000*std::sin(phase * 440 * 2*3.14 / 44100);
    data->right = 10000*std::sin(phase * 440 * 2*3.14 / 44100);
    data++;
    nscans--;
    phase++;
  }
  return data;
}

