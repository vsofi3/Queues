CFLAGS=-g -W -Wall -I/usr/local/include
LDFLAGS=-L/usr/local/lib
LIBRARIES=-lADTs
PROGRAMS=exerciseQueue llistqueue

all: $(PROGRAMS)

exerciseQueue: exerciseQueue.o
	gcc $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

llistqueue: llistqueue.o exerciseQueue.o
	gcc $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

exerciseQueue.o: exerciseQueue.c
llistqueue.o: llistqueue.c

clean:
	rm -f *.o $(PROGRAMS)
