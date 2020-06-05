CC      = gcc
CXX     = g++
CFLAGS  = -g -O3 -Wall -std=c++0x -pthread
LIBS    = -lpthread
LDFLAGS = -g

OBJECTS = FMParrot.o Parrot.o StopWatch.o Thread.o Timer.o

.PHONY: all
all:		FMParrot

FMParrot: ../Common/Common.a force	$(OBJECTS)
	$(CXX) $(OBJECTS) ../Common/Common.a $(CFLAGS) $(LIBS) -o FMParrot

-include $(OBJECTS:.o=.d)
%.o: %.cpp
	$(CXX) $(CFLAGS) -I../Common -c -o $@ $<
	$(CXX) -MM $(CFLAGS) -I../Common $< > $*.d

.PHONY: clean
clean:
	$(RM) FMParrot *.o *.d *.bak *~
	$(MAKE) -C ../Common clean

../Common/Common.a: force
	$(MAKE) -C ../Common

.PHONY: force
force :
	@true