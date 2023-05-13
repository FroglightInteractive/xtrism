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
  int rank = game->rank();
  int maxshow = std::min(int(rr.size()), 10);
  int h = height();
  int y0 = h/3;
  QStringList labels{"Rank", "Score", "Lines", "PPB", "Date"};
  QList<float> xx{   .15,    .25,    .35,     .45,     .5};
  auto drawrtext = [&p, &xx, x, y0, w, h](int i, float r, QString txt) {
    p.drawText(QRectF(QPointF(0,y0+r*h/30), QSizeF(x+w*(xx[i]+.05), h/30)),
               Qt::AlignRight, txt);
  };
  auto drawltext = [&p, &xx, x, y0, w, h](int i, float r, QString txt) {
    p.drawText(QRectF(QPointF(x+w*(xx[i]+.05),y0+r*h/30), QSizeF(w, h/30)),
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
  auto date = [](QString d) {
    QString year = d.mid(0,4);
    int m = d.mid(4,2).toInt() - 1;
    QString day = d.mid(6,2);
    QString hr = d.mid(9,2);
    QString mn = d.mid(11,2);
    if (m<0 || m>11)
      return QString("??");
    QStringList months{"Jan", "Feb", "Mar", "Apr", "May", "Jun",
      "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    if (day.startsWith("0"))
      day = day.mid(1);
    return months[m] + " " + day + ", " + year + ", " + hr + ":" + mn;
  };
        
  p.setPen(QColor(255,255,0));
  for (int i=0; i<labels.size(); i++)
    drawtext(i, -1.5, labels[i]);

  QString header =
   (rank==0 && maxshow>=5) ? "You scored your best score ever!"
    : (rank>=0 && maxshow>=5) ? "You scored one of your best scores ever!"
    :                "Your current and best scores:";
  p.drawText(QRectF(x + .1*w, y0 - h/10, w, h/10), Qt::AlignLeft, header);
  
  p.setPen(QColor(255,255,255));
  for (int r=0; r<maxshow; r++) {
      p.setPen(QColor(255,255,0));
    drawtext(0, r, QString::number(r+1) + ".");
    if (r!=rank)
      p.setPen(QColor(255,255,255));
    drawtext(1, r, QString::number(rr[r].score));
    drawtext(2, r, QString::number(rr[r].lines));
    drawtext(3, r, ppb(rr[r].score, rr[r].bricks));
    drawtext(4, r, r==rank ? QString("Just now!") : date(rr[r].date));
  }

  if (rank<0) {
    Score const &sc = game->score();
    p.setPen(QColor(255,255,0));
    float r = maxshow + .5;
    drawtext(0, r, "-- ");
    drawtext(1, r, QString::number(sc.score()));
    drawtext(2, r, QString::number(sc.lines()));
    drawtext(3, r, ppb(sc.score(), sc.bricks()));
    drawtext(4, r, "Just now");
  }
}

void HighScoreScreen::keyPressEvent(QKeyEvent *e) {
  // timeout?
  if (t0.elapsed() >= 500 || e->key()==Qt::Key_Escape)
    emit keyPressed();
}
