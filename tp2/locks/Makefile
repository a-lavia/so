CC = g++
CFLAGS = -g -Wall -Werror -std=c++11
LDFLAGS = -pthread

BINARIES = test

all: $(BINARIES)

test:
	$(CC) $(LDFLAGS) $(CFLAGS) test.cpp RWLock.cpp -o $(BINARIES)

clean:
	rm -f *.o
	rm -f $(BINARIES)