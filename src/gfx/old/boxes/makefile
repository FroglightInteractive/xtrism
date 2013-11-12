# Standard makefile for C/C++ projects

########################### OBJECTS AND  LIBRARIES ############################
OBJS =	box.o
SRCS =	*.C
LIBS =	$(CXLIBS) $(ALIBS) $(R6XLIBS) $(CLIBS)
INCS =	$(R6XINCDIR) $(AINCDIR)

#(# You might like to use $(ALIBS) $(R6XLIBS) and $(CLIBS) for LIBS #)#

################################ VARIOUS PATHS ################################
AINCDIR = -I/usr/X11R6/include
ALIBDIR = -L/usr/X11R6/lib
R6XINCDIR = -I/usr/X11R6/include
R6XLIBDIR = -L/usr/X11R6/lib
R6XLIBS = ${R6XLIBDIR} -lXt -lX11 -lXext -lSM -lICE
ALIBS = ${ALIBDIR} -lXaw -lfwf -lXmu -lXpm
CLIBS = -lm -lc
CXLIBS = -lcx

################################### FLAGS #####################################
CFLAGS = -g -Wall -W -Wno-inline -Woverloaded-virtual -Wno-unused \
	 -fhandle-exceptions -c ${PROFILE}
LFLAGS = -g ${PROFILE}
		#-Bsymbolic

########################### TOP LEVEL DEPENDENCIES ############################

boxes.a: boxes.a($(OBJS))

depend: $(SRCS)
	makedepend -Y -- $(CFLAGS) -- $(SRCS) 2> /dev/null

######################### OTHER CUSTOM DEPENDENCIES ###########################

########################### STANDARD DEPENDENCIES #############################
(%.o):	%.o
	ar cr $@ $<
%.o:	%.c
	echo -e '\n$<'
	gcc $(CFLAGS) -o $@ $< $(INCS)
%.o:	%.C
	echo -e '\n$<'
	g++ $(CFLAGS) -o $@ $< $(INCS)

# DO NOT DELETE

box.o: box.H
