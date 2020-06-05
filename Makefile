FMParrot/FMParrot: force
	$(MAKE) -C FMParrot

.PHONY: clean
clean:
	$(MAKE) -C FMParrot clean

.PHONY: force
force :
	@true