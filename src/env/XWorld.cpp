// XWorld.cpp

#include "XWorld.h"
#include <X11/Xlib.h>

class XWorldPrivate {
public:
  XWorldPrivate(QApplication *app, QWidget *widget) {
    QNativeInterface::QX11Application *x11
      = app->nativeInterface<QNativeInterface::QX11Application>();
    display = (Display*)x11->display();
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

bool XWorld::active() const {
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

XWorld::ID XWorld::storePixmap(RGBImage &img) {
  XImage *ximg = XCreateImage(d->display, d->visual, d->depth,
                              ZPixmap, 0,
                              (char*)img.data(),
                              img.width(), img.height(),
                              32, img.bytesperline());
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
  sizes[pixmap] = img.size();
  return pixmap;
}

void XWorld::renderPixmap(XWorld::ID id, int x, int y) {
  Q_ASSERT_X(sizes.contains(id), "renderPixmap", "Unknown pixmap");
  QSize size = sizes[id];
  XCopyArea(d->display, (Pixmap)id, d->window, d->gc,
            0, 0, size.width(), size.height(),
            x, y);
}

void XWorld::renderPixmap(XWorld::ID id, int x, int y, QRect src) {
  Q_ASSERT_X(sizes.contains(id), "renderPixmap", "Unknown pixmap");
  QSize size = sizes[id];
  XCopyArea(d->display, (Pixmap)id, d->window, d->gc,
            src.x(), src.y(), src.width(), src.height(),
            x, y);
}
