// TAlarmServer.H - subclass of THPoll serving alarms

#ifndef _TAlarmServer_H
#define _TAlarmServer_H

#include "TEnv.H"
#include "TTime.H"
#include <list.h>

class TAlarmEvent {
  public:
    TAlarmEvent(class TAlarm &a, TTime const &t): alarm(a), time(t) {}
    class TAlarm &alarm;
    TTime time;
    bool operator<=(TTime const &t) const { return time<t; }
  };

class TAlarmServer: public THPoll {
  public:
    TAlarmServer(TEnv &env);
    virtual ~TAlarmServer();
    virtual void tpoll(TTime const &time);
    void addalarm(class TAlarm &alrm, TTime const &time, bool killolds);
    void delalarms(class TAlarm &alrm);
    TTime const &first() { return first_; }
  private:
    list<TAlarmEvent> alarms;
    TEnv &tenv;
    TTime first_;
  };

#endif
