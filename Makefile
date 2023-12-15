CC = x86_64-w64-mingw32-gcc 

CFLAGS = -I/usr/x86_64-w64-mingw32/include 
SDL-CFLAGS = `/home/pi/SDL/x86_64-w64-mingw32/bin/sdl2-config --cflags` 
SDL-LIBS = `/home/pi/SDL/x86_64-w64-mingw32/bin/sdl2-config --libs`
LIBS = -lm -lSDL2 -lSDL2_mixer -lSDL2_ttf -lSDL2_image
OUTFILE = bark.exe

OBFILES = $(patsubst *.c, *.o, $(wildcard *.c))

# To build all, we have a prerequisite of bark.exe...
all:	bark.exe
	if [ -d Windows ]; then rm -rf Windows; fi
	mkdir Windows
	mv bark.exe *.png *.wav *.mp3 *.lvl Windows

bark.exe:	$(wildcard *.c)
	$(CC) $(CFLAGS) $(SDL-CFLAGS) $(SDL-LIBS) -o $(OUTFILE) $(wildcard *.c) $(LIBS)

# But in order to build bark.exe we need object files...
#bark.exe:	OBS $(OBFILES)
#	$(CC) $(CFLAGS) $(SDL-CFLAGS) $(SDL-LIBS) -o $(OUTFILE) $(wildcard *.o) $(LIBS) 

# So build the object files, which have a prerequisite of the .c files
#OBS:	$(wildcard *.c)
#	$(CC) $(CFLAGS) $(SDL-CFLAGS) -c $(wildcard *.c)

clean:
	rm *.o
