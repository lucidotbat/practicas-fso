CC=gcc
CFLAGS=-Wall -Wextra

all: init getty sh

init: init.c
	$(CC) $(CFLAGS) -o init init.c

getty: getty.c
	$(CC) $(CFLAGS) -o getty getty.c

sh: sh.c
	$(CC) $(CFLAGS) -o sh sh.c

clean:
	rm -f init getty sh
