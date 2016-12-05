ALL: src/Makefile
	+make src

src/Makefile: src/xtrism.pro src/xtrism.pri
	( cd src; qmake )
