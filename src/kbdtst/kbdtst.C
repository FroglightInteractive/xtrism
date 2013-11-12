#include "../env/TEnv.H"
#include "../env/TDrawCol.H"
#include <stdio.h>

class KbdTst: public THRedraw, public THKey {
  public:
    KbdTst(TEnv &tenv);
    ~KbdTst();
    virtual void tredraw(int x, int y, int w, int h);
    virtual void tkey(int code, bool io);
    void doredraw(int code);
  private:
    TEnv &tenv;
    bool keyin[128];
    TDrawCol tdc0, tdc1;
  };

KbdTst::KbdTst(TEnv &tenv0): tenv(tenv0),
  tdc0(tenv0,TRGB(0)), tdc1(tenv0,TRGB(255)) {
  tenv.reg_keyboard(this);
  tenv.reg_redraw(this);
  for (int i=0; i<128; i++)
    keyin[i]=0;
  }

KbdTst::~KbdTst() {
  tenv.dereg_redraw(this);
  tenv.dereg_keyboard(this);
  }

void KbdTst::tkey(int code, bool io) {
  if (code<0 || code>=128)
    { printf("code outside range: %i\n",code); return; }
  keyin[code]=io;
  if (code==9 && (keyin[50] || keyin[62]))
    for (int i=0; i<128; i++)
      { keyin[i]=0; doredraw(i); }
  printf("tkey: code %i %i\n",code,io);
  doredraw(code);
  }

void KbdTst::doredraw(int code) {
  (keyin[code]?tdc1:tdc0).rectangle(16*(code%16),16*(code/8),16,16,true);
  }

void KbdTst::tredraw(int x, int y, int w, int h) {
  for (int i=0; i<128; i++)
    doredraw(i);
  }    

int main(int argc, char *argv[]) {
  TEnv tenv(TReso(320,320,1,1),argc,argv,":0.0");
  { KbdTst kbdtst(tenv);
    tenv.loop();
  }
  return 0;
  }
