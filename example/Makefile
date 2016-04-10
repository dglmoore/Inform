UNAME=$(shell uname)

CCFLAGS=-Wall -Wextra -Wno-unused-parameter -O3 -I../include

ifeq ($(UNAME), Darwin)
LDFLAGS=-Wl,-flat_namespace,-undefined,dynamic_lookup
endif

all: test

remake: clean all

%.o: %.c ../include/ctest.h
	gcc $(CCFLAGS) -c -o $@ $<

test: main.o ../include/ctest.h mytests.o
	gcc $(LDFLAGS) main.o mytests.o -o test

clean:
	rm -f test *.o

