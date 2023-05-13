// MMBG.H - the image for the main menu

#ifndef _MMBG_H
#define _MMBG_H

#include <QPixmap>

class MMBG {
public:
  MMBG(QSize siz);
  QPixmap const &toPixmap() const;
private:
  QPixmap pm;
};

#endif
