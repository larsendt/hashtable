SRCDIR	= src
OBJDIR	= obj
CC 		= clang
CPP		= clang++
CFLAGS	= -Wall -Wextra -g -DDEBUG 
LFLAGS	= -L. -lhashtable -lcrypto 
VERSION	= 0.1

all: libhashtable.so hashtable-test

hashtable-test: $(SRCDIR)/main.cpp $(SRCDIR)/MD5Digest.h $(SRCDIR)/hashtable.h $(SRCDIR)/CPPHashTable.h libhashtable.so
	$(CPP) $(SRCDIR)/main.cpp $(LFLAGS) $(CFLAGS) -o hashtable-test

libhashtable.so: MD5Digest.o CPPHashTable.o hashtable.o
	$(CPP) $(CFLAGS) -fPIC -shared $(OBJDIR)/MD5Digest.o $(OBJDIR)/CPPHashTable.o $(OBJDIR)/hashtable.o -o libhashtable.so

MD5Digest.o: $(SRCDIR)/MD5Digest.cpp $(SRCDIR)/MD5Digest.h
	$(CPP) $(CFLAGS) -fPIC -c $(SRCDIR)/MD5Digest.cpp -o $(OBJDIR)/MD5Digest.o
	
CPPHashTable.o: $(SRCDIR)/CPPHashTable.cpp $(SRCDIR)/CPPHashTable.h
	$(CPP) $(CFLAGS) -fPIC -c $(SRCDIR)/CPPHashTable.cpp -o $(OBJDIR)/CPPHashTable.o

hashtable.o: $(SRCDIR)/hashtable.h $(SRCDIR)/hashtable.c
	$(CC) $(CFLAGS) -fPIC -c $(SRCDIR)/hashtable.c -o $(OBJDIR)/hashtable.o

clean:
	rm -f $(OBJDIR)/*.o
	rm -f hashtable-test
	rm -f libhashtable.so
