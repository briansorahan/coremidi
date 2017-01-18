SRCS = $(wildcard *.c)

coremidi: $(SRCS)
	@gcc -framework CoreFoundation -framework CoreMIDI -o coremidi *.c

install: coremidi
	@test -d $(HOME)/bin && cp coremidi $(HOME)/bin

clean:
	@rm -f coremidi

.PHONY: clean
