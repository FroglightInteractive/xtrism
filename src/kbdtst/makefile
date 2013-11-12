# Makefile for XTrism

######################## CONFIGURATION VARIABLES #############################
# (user editable)
# DEBUG = 
DEBUG = -g
BINDIR = /usr/local/bin
DATADIR = /usr/local/lib/xtrism

############################ OBJECTS AND LIBRARIES ###########################
#
OBJS = kbdtst.C

PARTS =	../env/lib.a \
	../basics/lib.a 

SUBDIRS = ../env ../basics

LIBS =	$(R6XLIBS) $(CLIBS)
INCS =	$(R6XINCDIR)

################################ VARIOUS PATHS ###############################
R6XINCDIR = -I/usr/X11R6/include
R6XLIBDIR = -L/usr/X11R6/lib
R6XLIBS = ${R6XLIBDIR} -lXt -lX11 -lXext -lSM -lICE
CLIBS = -lm -lc

################################### FLAGS ####################################
CFLAGS = ${DEBUG} -Wall -W -Wno-inline -Woverloaded-virtual -Wno-unused -c \
	 ${PROFILE} -m486
LFLAGS = ${DEBUG} -fhandle-exceptions ${PROFILE}
#		#-Bsymbolic
export DEBUG
export DATADIR

########################### TOP LEVEL DEPENDENCIES ###########################
kbdtst: subdirs $(OBJS)
	@echo -e '\nLinking kbdtst...'
	g++ $(LFLAGS) -o kbdtst $(OBJS) $(PARTS) $(LIBS)

depend: subdep
	makedepend -Y -- $(CFLAGS) -- $(SRCS) 2> /dev/null

######################### OTHER CUSTOM DEPENDENCIES ##########################
subdep:
	for i in $(SUBDIRS); do echo; $(MAKE) -e -C $$i depend || exit $?; \
		done

subdirs:
	for i in $(SUBDIRS); do echo; $(MAKE) -e -C $$i || exit $?; done

########################### STANDARD DEPENDENCIES ############################
%.o:	%.c
	@echo -e '\n$<'
	gcc $(CFLAGS) -o $@ $< $(INCS)
%.o:	%.C
	@echo -e '\n$<'
	g++ $(CFLAGS) -o $@ $< $(INCS)

########################## DYNAMIC DEPENDENCIES ##############################
# DO NOT DELETE
