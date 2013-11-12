// Pit_PlPl_Iface.H

#ifndef _Pit_PlPl_Iface_H
#define _Pit_PlPl_Iface_H

class PlPl_Pit_Iface {
  public:
    virtual void ok_to_land()=0;
    virtual void done_pudding(int lns)=0;
  };

class Game_PlPl_Iface {
  public:
    Pit_PlPl_Iface(): req(0), pud(0) { }
    virtual void request_to_land(PlPl_Pit_Iface *req0) { req=req0; }
    virtual void request_pudding(PlPl_Pit_Iface *pud0, int dir)
      { pud=pud0; }
    void grantrequest();
    void donepudding(int lns);
  private:
    PlPl_Pit_Iface *req, *pud;
  };

#endif
