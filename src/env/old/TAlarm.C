// TAlarm.C

#include "TAlarm.H"
#include "TAlarmServer.H"

TAlarm::~TAlarm() {
  alrm.delalarms(*this);
  }

void TAlarm::set(TTime const &time, bool killolds) {
  alrm.addalarm(*this,time,killolds);
  }
