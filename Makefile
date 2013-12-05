
INCLUDE=-Isrc -I$(TOOLS)/include/OpenEXR -I ../perlin-noise/src

LIBS=-lImath -lIlmImf -lpthread
LIBPATH=-L$(TOOLS)/lib

CPPFLAGS=-pg -Wall

all: testsuite

testsuite: testsuite/viewer

testsuite/viewer: testsuite/viewer.cpp
	g++ $(CPPFLAGS) testsuite/viewer.cpp -o testsuite/viewer $(INCLUDE) $(LIBS) $(LIBPATH) -lglut -lGL -lGLU

clean:
	rm -f testsuite/viewer

