// MainMenu.C

#include "MainMenu.H"

#include "../globals/Globals.H"
#include "../pics/MMBG.H"
#include "../env/TImageFile.H"
#include "../basics/Filename.H"
#include "../basics/minmax.H"
#include "../pics/MarbleBG.H"
#include "PlayButton.H"
#include "PlayerSelector.H"

#include "../basics/dbx.H"
#include "../pics/BoxMarbler.H"
#include "../popups/ListBox.H"
#include "../popups/InputField.H"

class ShadedTButton: public TextButton {
  public:
    ShadedTButton(GBParent *p, const Area &area, char const *txt,
                  class TImage &bg0, bool bgok0):
      TextButton(p,area,txt,tf()), bg(bg0), bgok(bgok0) {}
    virtual void redraw(BBox const &bb);
  private:
    class TImage &bg;
    bool bgok;
   };

void ShadedTButton::redraw(BBox const &bb) {
  if (!bgok)
    { BBox const &mybb=bbox();
      int wid=bg.width(), hei=bg.height();
      marblebg(width(),height(),2, // should be env.size dep.
               float(mybb.left())/wid,float(mybb.top())/hei,
               float(mybb.right())/wid,float(mybb.bottom())/hei,
               bg, mybb.left(), mybb.top());
      bgok=true;
      bg.partput(bb,bb.topleft()); // we can't be transp. the 1st time
    }
  TextButton::redraw(bb);
  }

MainMenu::MainMenu(): TopBox(tenv()) {  
  TImageFile timf(tenv(),(cachedir()+"mmbg").addpart(tenv().id()));
  backg=new MMBG(tenv(),timf.readable()?&timf:0);
  Area pbarea(tenv().actualfactor()*3,tenv().actualfactor()*2);
  playbuttons[0]=new SoloButton(this,pbarea,false,
			       "Solo Play", *backg,timf.readable());
  playbuttons[1]=new TeamButton(this,pbarea,
				"Team Play", *backg,timf.readable());
  playbuttons[2]=new ApartButton(this,pbarea,
				"Separate Play", *backg,timf.readable());
  playbuttons[3]=new SoloButton(this,pbarea,true,
				"Solo Play", *backg,timf.readable());

  textbut=new ShadedTButton(this,Area(int(tenv().width()*.75),
				      tenv().height()/24),
                            "X T R I S M", *backg,timf.readable());

  bms=new BoxMarblers(tenv(),128,64,max(tenv().width(),tenv().height()));

  for (int k=0; k<2; k++)
    selectors[k] = new PlayerSelector(this,Area(tenv().actualfactor()*3,
						tenv().actualfactor()),
				      k>0,
				      *backg,timf.readable(),
				      bms->find(TRGB(160,160,80)));
				      
  
  dbx(-20070110,"Mainmenu: post title");
  newchild(playbuttons[0],
           GBPos(this,1),GBPos(this,5),
	   GBPos(playbuttons[1],1),GBPos(this,1));
  newchild(playbuttons[1],
           GBPos(playbuttons[0],1),GBPos(this,5),
	   GBPos(playbuttons[2],1),GBPos(this,1));
  newchild(playbuttons[2],
           GBPos(playbuttons[1],1),GBPos(this,5),
	   GBPos(playbuttons[3],1),GBPos(this,1));
  newchild(playbuttons[3],
           GBPos(playbuttons[2],1),GBPos(this,5),
	   GBPos(this,1),GBPos(this,1));
  newchild(textbut,
           GBPos(playbuttons[0],1),GBPos(this,1),GBPos(playbuttons[3],1),
           GBPos(this,10));

  dbx(-20070110,"Mainmenu: mid children");

  newchild(selectors[0],
	   GBPos(this,1),GBPos(this,2),
	   GBPos(this,4),GBPos(this,10));
  newchild(selectors[1],
	   GBPos(this,4),GBPos(this,2),
	   GBPos(this,1),GBPos(this,10));

  dbx(-20070110,"Mainmenu: post children");

#if 0
  //+ larie
  //  Box3D *boxje=new Box3D(this,Area(tenv().width()/4,tenv().height()/5),
  //                         bms->find(TRGB(160,160,80)));
  ListBox *boxje = new ListBox(this,Area(tenv().width()/4,
					 ListBox::heightneeded(6,tf())),
			       tf(),tfyellow(),
			       bms->find(TRGB(160,160,80)));
  boxje->add("Item One");
  boxje->add("Item Two");
  boxje->add("Item Three");
  boxje->add("Item Four");
  boxje->add("Item Five");
  boxje->add("Item Six");
  boxje->add("Item Seven");
  boxje->add("Item Eight");
  boxje->add("Item Nine");
  boxje->add("Item Ten");
  boxje->add("Item Eleven");
  boxje->add("Item Twelve");
  boxje->add("Item Thirteen");
  boxje->add("Item Fourteen");
  boxje->add("Item Fifteen");
  boxje->add("Item Sixteen");
  //  inpf=new InputField(this,Area(tenv().width()/4-8,tenv().height()/10),
  //"Hello world",tf(),poll,tenv());
  newchild(boxje,GBPos(this,1),GBPos(this,1),GBPos(this,1),GBPos(this,5));
  //newchild(inpf,GBPos(this,1),GBPos(this,4),GBPos(this,1),GBPos(this,1));
  //- larie
#endif
  
  placechildren();
  dbx(-20070110,"Mainmenu:  children placed");

  if (!timf.readable())
    { draw(); timf.write(backg); }
  offerselect();
  }

MainMenu::~MainMenu() {
  dbx(1,"~MainMenu()");
  delete backg;
  delete textbut;
  delete playbuttons[0];
  delete playbuttons[1];
  delete playbuttons[2];
  delete playbuttons[3];

  //+ larie
  // delete bms;
  // delete boxje;
  //  delete inpf;
  //- larie
  }

void MainMenu::redraw(const BBox &bb) {
  backg->partput(bb,bb.topleft()); // use fact that my topleft=(0,0)
  TopBox::redraw(bb);
  }
