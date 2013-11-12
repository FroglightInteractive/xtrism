// Pit_PlPl_Iface.C

#include "Pit_PlPl_Iface.H"

void Pit_PlPl_Iface::grantrequest() {
  if (req)
    req->ok_to_land();
  req=0;
  }

void Pit_PlPl_Iface::donepudding(int lns) {
  if (pud)
    pud->done_pudding(lns);
  pud=0;
  grantrequest();
  }
