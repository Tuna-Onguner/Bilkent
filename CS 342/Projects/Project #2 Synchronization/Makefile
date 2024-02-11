CC = gcc
CFLAGS = -Wall -Wextra -pthread

LIBS = -lpthread -lrt

all: serverk clientk

serverk: serverk.o
	$(CC) $(CFLAGS) -o serverk serverk.o $(LIBS)

clientk: clientk.o
	$(CC) $(CFLAGS) -o clientk clientk.o $(LIBS)

serverk.o: serverk.c shareddefinitions.h
	$(CC) $(CFLAGS) -c serverk.c

clientk.o: clientk.c shareddefinitions.h
	$(CC) $(CFLAGS) -c clientk.c

clean:
	rm -f serverk clientk *.o
