// TAlarmServer.C

#include "TAlarmServer.H"
#include "TAlarm.H"

TAlarmServer::TAlarmServer(TEnv &env):
  tenv(env),
  first_(TTime::INFINITE_FUTURE) {
  tenv.reg_poll(this);
  }

TAlarmServer::~TAlarmServer() {
  tenv.dereg_poll(this);
  }

void TAlarmServer::tpoll(TTime const &time) {
  if (time>first_)
    return;
  list<TAlarmEvent>::iterator i,j;
  for (j=alarms.begin(); j!=alarms.end();)
    { i=j; j++;
      if ((*i)<=time)
        { (*i).alarm.alarmcall(time);
          alarms.erase(i);
        }
    }
  }

void TAlarmServer::addalarm(TAlarm &alrm, TTime const &time,
                            bool killolds) {
  if (killolds)
    delalarms(alrm);
  alarms.push_back(TAlarmEvent(alrm,time));
  if (time<first_)
    first_=time;
  }

void TAlarmServer::delalarms(TAlarm &alrm) {
  list<TAlarmEvent>::iterator i,j;
  for (j=alarms.begin(); j!=alarms.end();)
    { i=j; j++;
      if (&(*i).alarm==&alrm)
        alarms.erase(i);
    }
  }
