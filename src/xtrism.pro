TEMPLATE = app
TARGET = ../build/xtrism

include(xtrism.pri)

#DEFINES += TRISM_ESD
QT += multimedia widgets 
CONFIG += release
OBJECTS_DIR=../build/release
CONFIG(debug, debug|release) { OBJECTS_DIR=../build/debug }
MOC_DIR = $${OBJECTS_DIR}
RCC_DIR = $${OBJECTS_DIR}
UI_DIR = $${OBJECTS_DIR}
LIBS +=  -L/usr/X11R6/lib -lX11
