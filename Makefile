OPTS = -Wall -g -std=c++11 -Wno-deprecated
LIBS = -lGL -lglut -lm
ARCH := $(shell uname)
ifeq ($(ARCH), Linux)
else
 MACOSX_DEFINE = -DMACOSX -I/sw/include
 LIBS = -I/usr/common/include -I/usr/include/GL -L/System/Library/Frameworks/OpenGL.framework/Libraries -framework GLUT -framework OpenGL -framework Cocoa #-lGL -lm -lobjc -lstdc++ -lGLU lGL #-lglut
endif



brawl: main.o Critter.o Player.o Platform.o Bird.o
	g++ $(OPTS) -o brawl main.o Critter.o Player.o Platform.o Bird.o $(LIBS)

main.o: main.cpp Critter.h Player.h Platform.h Bird.h glutfuncs.h constants.h
	g++ $(OPTS) $(MACOSX_DEFINE) -c main.cpp


Critter.o:	Critter.h Critter.cpp Platform.h constants.h
	g++ $(OPTS) -c Critter.cpp

Player.o:	Player.h Player.cpp Critter.h constants.h
	g++ $(OPTS) -c Player.cpp

Bird.o:		Bird.h Bird.cpp Critter.h constants.h
	g++ $(OPTS) -c Bird.cpp


Platform.o:	Platform.h Platform.cpp Critter.h constants.h
	g++ $(OPTS) -c Platform.cpp


clean:
	rm -f *.o brawl *~
