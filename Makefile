SRCDIR	= src
OBJDIR	= obj
CC 		= clang
CPP		= clang++
CFLAGS	= -Wall -Wextra -g -DDEBUG -DTEST 
LFLAGS	= -L. -lhashtable -lcrypto -lrt
VERSION	= 0.1

all: libhashtable.so hashtable-test

hashtable-test: $(SRCDIR)/main.c $(SRCDIR)/hashtable.h libhashtable.so
	$(CC) $(SRCDIR)/main.c $(LFLAGS) $(CFLAGS) -o hashtable-test

libhashtable.so: hashtable.o 
	$(CC) $(CFLAGS) -fPIC -shared $(OBJDIR)/hashtable.o -o libhashtable.so

hashtable.o:
	$(CC) $(CFLAGS) -fPIC -c $(SRCDIR)/hashtable.c -o $(OBJDIR)/hashtable.o

clean:
	rm -f $(OBJDIR)/*.o
	rm -f hashtable-test
	rm -f libhashtable.so
