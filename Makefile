SRCDIR	= src
CC 		= gcc
CFLAGS	= -Wall -Wextra -g -DDEBUG -DTEST
LFLAGS	= -lrt -L. -lhashtable

all: hashtable-test hashtable-lib

hashtable-lib: $(SRCDIR)/hashtable.h $(SRCDIR)/hashtable.c $(SRCDIR)/murmur.h $(SRCDIR)/murmur.c
	$(CC) $(CFLAGS) $(SRCDIR)/hashtable.c $(SRCDIR)/murmur.c -fPIC -rdynamic -shared -o libhashtable.so

hashtable-static-lib: $(SRCDIR)/hashtable.h $(SRCDIR)/hashtable.c $(SRCDIR)/murmur.h $(SRCDIR)/murmur.c
	$(CC) $(CFLAGS) -c $(SRCDIR)/hashtable.c $(SRCDIR)/murmur.c
	ar crf libhashtable.a hashtable.o murmur.o

hashtable-test: $(SRCDIR)/main.c hashtable-lib
	$(CC) $(SRCDIR)/main.c $(LFLAGS) $(CFLAGS) -o hashtable-test

docs:
	doxygen doxygen-hashtable.conf

clean:
	rm -f *.o
	rm -f hashtable-test
	rm -f libhashtable.so
	rm -f libhashtable.a
	rm -rf docs
