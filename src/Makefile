# Makefile for XTrism

######################## CONFIGURATION VARIABLES #############################
# (user editable)
# DEBUG = 
DEBUG = -g
BINDIR = /usr/local/bin
DATADIR = /usr/local/lib/xtrism

TRISM_ESD = false
#true
SND = dsp
#SND = esd

############################ OBJECTS AND LIBRARIES ###########################
#
PARTS = main/lib.a \
	mainmenu/lib.a \
	popups/lib.a \
	sessions/lib.a \
	globals/lib.a \
	game/lib.a \
	gfx/lib.a \
	options/lib.a \
	bricks/lib.a \
	pics/lib.a \
	bytemap/lib.a \
	sound/lib.a \
	poll/lib.a \
	env/lib.a \
	basics/lib.a 

SUBDIRS = basics env poll sound esd dsp bytemap pics bricks options \
	gfx game globals sessions popups mainmenu main

STRIP = basics env bytemap options gfx sound bricks mainmenu 

DEFDATA = bricks glopts players probs

LIBS =	-lesd $(R6XLIBS) $(CLIBS)
INCS =	$(R6XINCDIR)

################################ VARIOUS PATHS ###############################
R6XINCDIR = -I/usr/X11R6/include
R6XLIBDIR = -L/usr/X11R6/lib
R6XLIBS = ${R6XLIBDIR} -lXt -lX11 -lXext -lSM -lICE
CLIBS = -lm -lc -I/usr/include/g++

################################### FLAGS ####################################
CFLAGS = ${DEBUG} -Wall -W -Wno-inline -Woverloaded-virtual -Wno-unused -c \
	 ${PROFILE} -m486
LFLAGS = ${DEBUG} -fhandle-exceptions ${PROFILE}
#		#-Bsymbolic
export DATADIR
export STRIP

########################### TOP LEVEL DEPENDENCIES ###########################
xtrism: subdirs
	@echo -e '\nLinking xtrism...'
	g++ $(LFLAGS) -o xtrism $(PARTS) $(SND)/lib.a $(LIBS)

depend: subdep
	makedepend -Y -- $(CFLAGS) -- $(SRCS) 2> /dev/null

install: xtrism
	cp xtrism ${BINDIR}
	strip ${BINDIR}/xtrism
	chmod ugo=rx ${BINDIR}/xtrism
	mkdir -p ${DATADIR}
	cd data
	cp ${DEFDATA} ${DATADIR}
	cd ..

config: datadir dellibs makefiles
makefiles: submake 

clean:;	mkclean

backup:; mkbackup

######################### OTHER CUSTOM DEPENDENCIES ##########################
subdep:
	for i in $(SUBDIRS); do echo; $(MAKE) -e -C $$i depend || exit $?; \
		done

subdirs:
	for i in $(SUBDIRS); do echo; $(MAKE) -e -C $$i || exit $?; done

dellibs:
	for i in $(SUBDIRS); do \
	rm -f $$i/lib.a; \
	done

submake:
	for i in $(SUBDIRS); do \
		echo -e '\n'; \
		cd $$i; \
		../mkimake $(STRIP); \
		imake -T../Imake.tmpl || exit $?; \
		make depend || exit $?; \
		cd ..; \
		done

datadir:
	echo "#define DATADIR \"${DATADIR}\"" > .datadir

########################### STANDARD DEPENDENCIES ############################
%.o:	%.c
	@echo -e '\n$<'
	gcc $(CFLAGS) -o $@ $< $(INCS)
%.o:	%.C
	@echo -e '\n$<'
	g++ $(CFLAGS) -o $@ $< $(INCS)

########################## DYNAMIC DEPENDENCIES ##############################
# DO NOT DELETE
