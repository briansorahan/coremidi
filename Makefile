SRCS = $(wildcard *.c)

coremidi: $(SRCS)
	@gcc -framework CoreFoundation -framework CoreMIDI -o coremidi *.c

install: coremidi
	@if [[ -d $(HOME)/bin ]]; then \
             install coremidi $(HOME)/bin; \
         elif [[  -d /usr/local/bin ]]; then \
             install  coremidi /usr/local/bin; \
	 fi

clean:
	@rm -f coremidi

.PHONY: clean
