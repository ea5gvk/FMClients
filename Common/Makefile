OBJECTS = FMNetwork.o UDPSocket.o

.PHONY:		all
all:		Common.a

Common.a:	$(OBJECTS)
	$(AR) rcs Common.a $(OBJECTS)

.PHONY: clean
clean:
	$(RM) Common.a *.o *.d *.bak *~

.PHONY: force
force :
	@true