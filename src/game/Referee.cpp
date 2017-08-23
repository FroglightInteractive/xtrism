// Referee.C

#include "Referee.H"
#include "Kronos.H"
#include "../env/TTime.H"
#include <math.h>
#include "../basics/Throw.H"

/* ------------------------------ brickprepare ---------------------------- */
void Referee::brickprepare(int y_drp, bool selfdrp) {
  dt_godown = kr.dt_godown();
  dt_used = kr.dt_used();
  y_drop = y_drp;
  selfdrop = selfdrp;
  prepd = true;
}

/* ------------------------------ brickscore ------------------------------ */
/* In the present vsn, the args are ignored: the score is computed
   using dt_godown and dt_used from Kronos only.
 */
double Referee::brickscore(int y_land) {
  fthrow(prepd, "Referee: Not prepared");
  prepd = false;
  return 15000000 / ((dt_godown + 350) * (dt_used + 200));
}

/* ------------------------------ linesscore ------------------------------ */
double Referee::linesscore(int lines) {
  return 0;
}