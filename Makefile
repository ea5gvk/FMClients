.PHONY: all
all:  FMParrot/FMParrot FM2DStar/FM2DStar

FM2DStar/FM2DStar: force
	$(MAKE) -C FM2DStar

FMParrot/FMParrot: force
	$(MAKE) -C FMParrot

.PHONY: clean
clean:
	$(MAKE) -C FM2DStar clean
	$(MAKE) -C FMParrot clean

.PHONY: force
force :
	@true