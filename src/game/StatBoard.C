// StatBoard.C

#include "StatBoard.H"
#include "../env/TImage.H"
#include "../env/TFont.H"
#include "../bytemap/BMRecolour.H"

#include "../env/TBusy.H"

const int INTERL=2;
const int EDGEMARG=4;
const int CENTREMARG=4;
const int TBMARG=4;

StatBoard::StatBoard(GBParent *p, int nlines, int labelw, int dataw,
                     TFont const &labelf, TFont const &dataf,
                     SharedBG &sbg, class PollServer &syncserv):
  GBox(p, Area(labelw+dataw+2*EDGEMARG+CENTREMARG,
	       nlines*(max(labelf.height(),dataf.height())+INTERL)+
	       2*TBMARG-INTERL)),
  BGSharer(sbg), Sleeper(syncserv),
  numlines(nlines), labelwidth(labelw), datawidth(dataw),
  labelfont(labelf), datafont(dataf) ,
  updlabels(0), upddata(0) {
  dy=max(labelf.height(),dataf.height())+INTERL;
  dy0=TBMARG+dataf.ascent();
  dxl=dxd=EDGEMARG;
  }

void StatBoard::setlabel(int i, string const &txt, bool update) {
  ensure(i);
  labels[i]=txt;
  //int ty=top()+dy0+i*dy;
  //  int lx=left()+dxl;
  updlabels|=(1<<i);
  if (update)
    warn();
  }

void StatBoard::setdata(int i, string const &txt, bool update) {
  ensure(i);
  data[i]=txt;
  //  int ty=top()+dy0+i*dy;
  //int rx=right()-dxd;
  upddata|=(1<<i);
  if (update)
    { warn(); }
  }

#include <stdio.h>

void StatBoard::setdata(int i, int val, bool update) {
  char buf[10];
  sprintf(buf,"%i",val);
  setdata(i,buf,update);
  }

void StatBoard::setdata(int i, double val, bool update) {
  char buf[10];
  sprintf(buf,"%.1f",val);
  setdata(i,buf,update);
  }

bool StatBoard::draw(Point const &origin, class ByteMap *bm) {
  int bw=width()/75+1;
  BBox bb=bbox(); bb.shift(-origin.x(),-origin.y());
  recolour_rectangle(bm, bb, .75,0);
  recolour_rect_edges(bm, bb, bw,-64);
  return true;
  }

void StatBoard::redraw(const BBox &bbox) {
  BGSharer::redraw(bbox);
  for (unsigned int i=0; i<labels.size(); i++)
    redrawlabel(i,false);
  for (unsigned int i=0; i<data.size(); i++)
    redrawdata(i,false);
  }

void StatBoard::poll() {
  int i=0;
  word w=1;
  while (updlabels)
    { if (updlabels & w)
      { redrawlabel(i,true); updlabels&=~w; }
      i++; w<<=1;
    }
  i=0; w=1;
  while (upddata)
    { if (upddata & w)
      { redrawdata(i,true); upddata&=~w; }
      i++; w<<=1;
    }
  }

void StatBoard::redrawlabel(int i, bool bg) {
  if (bg)
    BGSharer::redraw(BBox(Point(left()+dxl,top()+dy0+dy*i-labelfont.ascent()),
                          Area(labelwidth,labelfont.height())));
  labelfont.write(labels[i].c_str(),
                  left()+dxl, top()+dy0+dy*i,
                  TFont::LEFT, 0,labelwidth);
  }

void StatBoard::redrawdata(int i, bool bg) {
  if (bg)
    BGSharer::redraw(BBox(Point(right()-dxd-datawidth,
                                top()+dy0+dy*i-datafont.ascent()),
                          Area(labelwidth,datafont.height())));
  datafont.write(data[i].c_str(),
                 right()-dxd, top()+dy0+dy*i,
                 TFont::RIGHT, 0,datawidth);
  }

void StatBoard::cleardata() {
  for (int i=0; i<numlines; i++)
    setdata(i,"");
  }

void StatBoard::ensure(unsigned int i) {
  while (labels.size() <= i)
    labels.push_back("");
  while (data.size() <= i)
    data.push_back("");
  }
