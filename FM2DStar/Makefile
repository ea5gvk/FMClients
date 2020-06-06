CC      = gcc
CXX     = g++
CFLAGS  = -g -O3 -Wall -std=c++0x -pthread
LIBS    = -lpthread
LDFLAGS = -g

OBJECTS = Conf.o FM2DStar.o

.PHONY: all
all:		FM2DStar

FM2DStar: ../Common/Common.a GitVersion.h  force $(OBJECTS)
	$(CXX) $(OBJECTS) ../Common/Common.a $(CFLAGS) $(LIBS) -o FM2DStar

-include $(OBJECTS:.o=.d)
%.o: %.cpp
	$(CXX) $(CFLAGS) -I../Common -c -o $@ $<
	$(CXX) -MM $(CFLAGS) -I../Common $< > $*.d

.PHONY: clean
clean:
	$(RM) FMParrot GitVersion.h *.o *.d *.bak *~
	$(MAKE) -C ../Common clean

../Common/Common.a: force
	$(MAKE) -C ../Common

# Export the current git version if the index file exists, else 000...
GitVersion.h:
ifneq ("$(wildcard ../.git/)","")
	echo "const char *gitversion = \"$(shell git rev-parse HEAD)\";" > $@
else
	echo "const char *gitversion = \"0000000000000000000000000000000000000000\";" > $@
endif

.PHONY: force
force :
	@true