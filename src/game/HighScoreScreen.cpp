// HighScoreScreen.cpp

#include "HighScoreScreen.h"
#include <algorithm>
#include "Score.h"
#include "Records.h"
#include <QPainter>
#include <QKeyEvent>

HighScoreScreen::HighScoreScreen(NiceGame *game,
                                 QPixmap bg, QWidget *parent):
  QWidget(parent),
  game1(game), game2(0), bg(bg) {
  t0.start();
}

HighScoreScreen::HighScoreScreen(NiceGame *game1, NiceGame *game2,
                                 QPixmap bg, QWidget *parent):
  QWidget(parent),
  game1(game1), game2(game2), bg(bg) {
  t0.start();
}

HighScoreScreen::~HighScoreScreen() {
}

void HighScoreScreen::paintEvent(QPaintEvent *) {
  QPainter p(this);
  p.drawPixmap(0, 0, bg);

  if (game2) {
    renderGame(p, game1, 0, width()/2);
    renderGame(p, game2, width()/2, width()/2);
  } else {
    renderGame(p, game1, width()/4, width()/2);
  }
}

void HighScoreScreen::renderGame(QPainter &p, NiceGame *game, int x, int w) {
  Records const &rr(AllRecords::instance()
                    [game->recordName()][game->brickset()]);
  Record const &rec = game->finalScore();
  int nyear = AllRecords::instance(AllRecords::Era::ThisYear)
    [game->recordName()][game->brickset()].size();
  Record const &bestyear = AllRecords::instance(AllRecords::Era::ThisYear)
    [game->recordName()][game->brickset()][0]; // exists, because game inserted
  int ntoday = AllRecords::instance(AllRecords::Era::Today)
    [game->recordName()][game->brickset()].size();
  Record const &besttoday = AllRecords::instance(AllRecords::Era::Today)
    [game->recordName()][game->brickset()][0]; // exists, because game inserted
    
  int rank = game->rank();
  int maxshow = std::min(int(rr.size()), 10);
  int h = height();
  int y0 = h/3;
  auto yforr = [h, y0](float r) { return y0 + r*h/40; };
  QStringList labels{"Rank", "Score", "Lines", "PPB", "Date"};
  QList<float> xx{   .15,    .25,    .35,     .45,     .5};
  auto drawrtext = [&p, &xx, x, yforr, w, h](int i, float r, QString txt) {
    p.drawText(QRectF(0, yforr(r), x+w*(xx[i]+.05), h/30),
               Qt::AlignRight, txt);
  };
  auto drawltext = [&p, &xx, x, yforr, w, h](int i, float r, QString txt) {
    p.drawText(QRectF(x+w*(xx[i]+.05), yforr(r), w, h/30),
               Qt::AlignLeft, txt);
  };
  auto drawtext = [&p, &xx, &drawltext, &drawrtext, x, w, h]
    (int i, float r, QString txt) {
    if (i==4)
      drawltext(i, r, txt);
    else
      drawrtext(i, r, txt);
  };
  auto ppb = [](int score, int bricks) {
    return bricks ? QString::number(score*1.0/bricks, 'f', 1) : "-";
  };
  auto date = [](QString d, bool withyear=true) {
    QString year = d.mid(0,4);
    int m = d.mid(4,2).toInt();
    QString day = d.mid(6,2);
    QDate today = QDate::currentDate();
    if (today.year()==year.toInt() && today.month()==m
        && today.day()==day.toInt())
      return QString("Today");
    if (m<1 || m>12)
      return QString("??");
    QStringList months{"Jan", "Feb", "Mar", "Apr", "May", "Jun",
      "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    if (day.startsWith("0"))
      day = day.mid(1);
    QString res = months[m-1] + " " + day;
    if (withyear)
      res += ", " + year;
    return res;
  };
  auto time = [](QString d) {
    QString hr = d.mid(9,2);
    QString mn = d.mid(11,2);
    return hr + ":" + mn;
  };
        
  QString header;
  if (rank==0 && maxshow>=5) {
    header = "You scored your best score ever!";
  } else if (rank>=0 && maxshow>=5) {
    header = "You scored ";
    if (rec==bestyear) 
      header += "your best score this year and ";
    header += "one of your best scores ever!";
  } else if (nyear>=5 && rec==bestyear) {
    header = "You scored your best score this year!";
  } else if (ntoday>=5 && rec==besttoday) {
    header = "You scored your best score today!";
  } else {
    header = "";
  }
  int xhead = x + .1*w;
  p.setPen(QColor(255,255,0));
  p.drawText(QRectF(xhead+.1*w, yforr(-5), w, h/30), Qt::AlignLeft, header);
  
  p.setPen(QColor(255,255,0));

  p.drawText(QRectF(xhead, yforr(-2.6), w, h/30), Qt::AlignLeft, "Your best scores:");
  for (int i=0; i<labels.size(); i++)
    drawtext(i, -1.3, labels[i]);

  p.setPen(QColor(255,255,255));
  for (int r=0; r<maxshow; r++) {
      p.setPen(QColor(255,255,0));
      drawtext(0, r, QString("%1.").arg(r+1));
    if (r!=rank)
      p.setPen(QColor(255,255,255));
    drawtext(1, r, QString::number(rr[r].score));
    drawtext(2, r, QString::number(rr[r].lines));
    drawtext(3, r, ppb(rr[r].score, rr[r].bricks));
    drawtext(4, r, r==rank ? QString("Just now!") : date(rr[r].date));
  }

  float r = maxshow + .7;
  if ((rank<0 || rank>=maxshow) && rec!=bestyear) {
    p.setPen(QColor(255,255,0));
    p.drawText(QRectF(xhead, yforr(r), w, h/30), Qt::AlignLeft,
               "Best this year:");
    r += 1.3;
    int idx = rr.indexOf(bestyear);
    drawtext(0, r, (idx>=maxshow) ? QString("%1.").arg(idx+1) : QString("-- "));
    p.setPen(QColor(255,255,255));
    drawtext(1, r, QString::number(bestyear.score));
    drawtext(2, r, QString::number(bestyear.lines));
    drawtext(3, r, ppb(bestyear.score, bestyear.bricks));
    QString d = date(bestyear.date, false);
    if (d=="Today")
      p.setPen(QColor(255,255,0));
    drawtext(4, r, d);
    r += 1.7;
  }

  if ((rank<0 || rank>=maxshow) && rec!=besttoday && besttoday!=bestyear) {
    p.setPen(QColor(255,255,0));
    p.drawText(QRectF(xhead, yforr(r), w, h/30), Qt::AlignLeft,
               "Best today:");
    r += 1.3;
    int idx = rr.indexOf(besttoday);
    drawtext(0, r, (idx>=maxshow) ? QString("%1.").arg(idx+1) : QString("-- "));
    p.setPen(QColor(255,255,255));
    drawtext(1, r, QString::number(besttoday.score));
    drawtext(2, r, QString::number(besttoday.lines));
    drawtext(3, r, ppb(besttoday.score, besttoday.bricks));
    drawtext(4, r, time(besttoday.date));
    r += 1.7;
  }

  if (rank<0 || rank>=maxshow) {
    p.setPen(QColor(255,255,0));
    p.drawText(QRectF(xhead, yforr(r), w, h/30), Qt::AlignLeft,
               "This game:");
    r += 1.3;
    drawtext(0, r, rank<0 ? QString("-- ") : QString("%1.").arg(rank+1));
    drawtext(1, r, QString::number(rec.score));
    drawtext(2, r, QString::number(rec.lines));
    drawtext(3, r, ppb(rec.score, rec.bricks));
    drawtext(4, r, "Just now");
  }
}

void HighScoreScreen::keyPressEvent(QKeyEvent *e) {
  if (t0.elapsed() < 500) {
    int code = e->nativeScanCode();
    if (game1->allKeyCodes().contains(code))
      return;
    if (game2 && game2->allKeyCodes().contains(code))
      return;
  }
  emit keyPressed();
}
