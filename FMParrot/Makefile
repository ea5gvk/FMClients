CC      = gcc
CXX     = g++
CFLAGS  = -g -O3 -Wall -std=c++0x -pthread
LIBS    = -lpthread
LDFLAGS = -g

OBJECTS = FMNetwork.o FMParrot.o Parrot.o StopWatch.o Thread.o Timer.o UDPSocket.o

all:		FMParrot

FMParrot:	$(OBJECTS)
		$(CXX) $(OBJECTS) $(CFLAGS) $(LIBS) -o FMParrot

%.o: %.cpp
		$(CXX) $(CFLAGS) -c -o $@ $<

install:
		install -m 755 FMParrot /usr/local/bin/

clean:
		$(RM) FMParrot *.o *.d *.bak *~
