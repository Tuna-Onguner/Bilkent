CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lrt -lm


all: primeP primeT


primeP: primeP.c
	$(CC) $(CFLAGS) -o primeP primeP.c $(LDFLAGS)

primeT: primeT.c
	$(CC) $(CFLAGS) -o primeT primeT.c -pthread $(LDFLAGS)


clean:
	rm -f primeP primeT


.PHONY: all clean
