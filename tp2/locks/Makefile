CC = g++
CFLAGS = -g -Wall -Werror -std=c++11
LDFLAGS = -pthread

BINARIES = RWLockTestCuatriPasado

all: $(BINARIES)

RWLock: 
	$(CC) $(LDFLAGS) $(CFLAGS) RWLockTestCuatriPasado.cpp RWLock.cpp -o RWLockTestCuatriPasado

clean:
	rm -f *.o
	rm -f $(BINARIES)