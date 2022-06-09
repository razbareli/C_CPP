CC = gcc
CFLAGS = -Wall -Wextra -Wvla -Werror -g -std=c99

pair.o: pair.c pair.h
	$(CC) -c $(CFLAGS) $<

vector.o: vector.c vector.h
	$(CC) -c $(CFLAGS) $<

hashmap.o: hashmap.c hashmap.h vector.h pair.h
	$(CC) -c $(CFLAGS) $<

test_suite.o: test_suite.c test_suite.h hash_funcs.h test_pairs.h hashmap.h vector.h pair.h
	$(CC) -c $(CFLAGS) $<

libhashmap_tests.a: test_suite.o
	ar rcs libhashmap_tests.a test_suite.o

libhashmap.a: hashmap.o
	ar rcs libhashmap.a hashmap.o
clean:
	rm -f *.o *.a
all: libhashmap.a libhashmap_tests.a

.PHONY: all, clean