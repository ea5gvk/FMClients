OBJECTS = FMNetwork.o Log.o StopWatch.o Thread.o UDPSocket.o

.PHONY:		all
all:		Common.a

Common.a:	$(OBJECTS)
	$(AR) rcs Common.a $(OBJECTS)

-include $(OBJECTS:.o=.d)
%.o: %.cpp
		$(CXX) $(CFLAGS) -c -o $@ $<
		$(CXX) -MM $(CFLAGS) $< > $*.d

.PHONY: clean
clean:
	$(RM) Common.a *.o *.d *.bak *~

.PHONY: force
force :
	@true