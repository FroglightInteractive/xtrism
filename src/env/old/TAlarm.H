// TAlarm.H - client for TAlarmServer

#ifndef _TAlarm_H
#define _TAlarm_H

#include "TEnv.H"

class TAlarm {
  public:
    TAlarm(class TAlarmServer &al): alrm(al) { }
    virtual ~TAlarm();
    virtual void alarmcall(class TTime const &time)=0;
    void set(class TTime const &time, bool killolds=false);
  private:
    class TAlarmServer &alrm;
  };

#endif
