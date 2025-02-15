# Copyright 2023 Echipa PCLP1

# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# define targets
TARGETS = my_octave

build: $(TARGETS)

my_octave: my_octave.c
	$(CC) $(CFLAGS) my_octave.c -o my_octave

pack:
	zip -FSr 3XYCA_FirstnameLastname_Tema2.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
