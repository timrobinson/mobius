MAKEFILE=tc.mak
MODEL	=l
CFLAGS	=-v -w -m$(MODEL) -O2 -1 -d -Z -vi -f-
LFLAGS	=/v /x /Lc:\tc\lib

OBJS	=play.o misc.o irq.o dma.o			\
	wss.o sb.o					\
	wav.o wav01.o wav02.o wav06.o wav07.o wav34.o	\
	wav49.o wav85.o au.o au01.o au023.o au27.o

all:	play.exe

.c.o:
	tcc $(CFLAGS) -c -o$@ $<

play.exe: $(OBJS) $(MAKEFILE)
	tlink $(LFLAGS) @&&!
c0$(MODEL) $(OBJS)
$@
	# no map file (tlink /x)
c$(MODEL).lib
!

dma.o: dma.c $(MAKEFILE)

irq.o: irq.c defs.h irq.h $(MAKEFILE)

wss.o: wss.c defs.h irq.h $(MAKEFILE)

play.o: play.c defs.h $(MAKEFILE)

wav.o: wav.c defs.h $(MAKEFILE)

wav01.o: wav01.c defs.h $(MAKEFILE)

wav02.o: wav02.c defs.h $(MAKEFILE)

wav06.o: wav06.c defs.h $(MAKEFILE)

wav07.o: wav07.c defs.h $(MAKEFILE)

wav34.o: wav34.c defs.h $(MAKEFILE)

wav49.o: wav49.c defs.h $(MAKEFILE)

wav85.o: wav85.c defs.h $(MAKEFILE)

au.o:	au.c defs.h $(MAKEFILE)

au01.o: au01.c defs.h $(MAKEFILE)

au023.o: au023.c defs.h $(MAKEFILE)

au27.o: au27.c defs.h $(MAKEFILE)

clean:
	del *.o
	del *.exe