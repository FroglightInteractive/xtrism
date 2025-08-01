// XWorld.cpp

#include "XWorld.h"
#include <X11/Xlib.h>
#include <stdlib.h>

#if QT_VERSION < 0x060000
#include <QX11Info>
#endif

class XWorldPrivate {
public:
  XWorldPrivate(QApplication *app, QWidget *widget) {
    display = 0;
#if QT_VERSION >= 0x060000
    QNativeInterface::QX11Application *x11
      = app->nativeInterface<QNativeInterface::QX11Application>();
    if (x11)
      display = (Display*)x11->display();
#else
    display = QX11Info::display();
#endif
    if (!display) {
      qDebug() << "Cannot run without an X11 display";
      qDebug() << "When running under Wayland, try";
      qDebug();
      qDebug() << "    export QT_QPA_PLATFORM=xcb";
      exit(1);
    }
    screen = XDefaultScreen(display);
    depth = DefaultDepth(display, screen);
    visual = XDefaultVisual(display, screen);
    window = (Drawable)widget->effectiveWinId();
    XGCValues gcv;
    gc = XCreateGC(display, window, 0, &gcv);
  }
  ~XWorldPrivate() {
    XFreeGC(display, gc);
  }
public:
  Display *display;
  int screen;
  int depth;
  Visual *visual;
  Drawable window;
  GC gc;
};

bool XWorld::isActive() const {
  return d->display && d->visual && d->window && d->gc;
}

XWorld::XWorld(QApplication *app, QWidget *widget):
  d(new XWorldPrivate(app, widget)) {
}

XWorld::~XWorld() {
  for (ID pixmap: sizes.keys()) 
    XFreePixmap(d->display, (Pixmap)pixmap);
  delete d;
}

void XWorld::storePixmap(QPixmap const *pm) {
  QImage img(pm->toImage());
  XImage *ximg = XCreateImage(d->display, d->visual, d->depth,
                              ZPixmap, 0,
                              (char*)img.bits(),
                              img.width(), img.height(),
                              32, img.bytesPerLine());
  Q_ASSERT_X(ximg, "storePixmap", "XImage creation failed");
  ID pixmap = (ID)XCreatePixmap(d->display, d->window,
                                img.width(), img.height(),
                                d->depth);
  Q_ASSERT_X(pixmap, "storePixmap", "Pixmap creation failed");
  XPutImage(d->display, pixmap, d->gc, ximg,
            0, 0, 0, 0,
            img.width(), img.height());
  // not XDestroyImage(ximg); this would clobber my RGBImage
  XFree(ximg); // is this OK?
  pixmaps[pm] = pixmap;
  sizes[pixmap] = img.size();
}

void XWorld::renderPixmap(QPixmap const *pm, int x, int y) {
  Q_ASSERT_X(pixmaps.contains(pm), "renderPixmap", "Unknown pixmap");
  ID id = pixmaps[pm];
  QSize size = sizes[id];
  XCopyArea(d->display, (Pixmap)id, d->window, d->gc,
            0, 0, size.width(), size.height(),
            x, y);
}

void XWorld::renderPixmap(QPixmap const *pm, int x, int y, QRect src) {
  Q_ASSERT_X(pixmaps.contains(pm), "renderPixmap", "Unknown pixmap");
  ID id = pixmaps[pm];
  QSize size = sizes[id];
  XCopyArea(d->display, (Pixmap)id, d->window, d->gc,
            src.x(), src.y(), src.width(), src.height(),
            x, y);
}

void XWorld::dropPixmap(QPixmap const *pm) {
  Q_ASSERT_X(pixmaps.contains(pm), "dropPixmap", "Unknown pixmap");
  ID id = pixmaps[pm];
  XFreePixmap(d->display, (Pixmap)id);
  pixmaps.remove(pm);
  sizes.remove(id);
}

void XWorld::flush() {
  XFlush(d->display);
}
