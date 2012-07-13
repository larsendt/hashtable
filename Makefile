SRCDIR	= src
OBJDIR	= obj
CC 		= clang++
CFLAGS	= -Wall -Wextra -g 
LFLAGS	= -L. -lhashtable -lcrypto
VERSION	= 0.1

all: libhashtable.so hashtable-test

hashtable-test: $(SRCDIR)/main.cpp $(SRCDIR)/MD5Digest.h $(SRCDIR)/HashTable.h libhashtable.so
	$(CC) $(SRCDIR)/main.cpp $(LFLAGS) $(CFLAGS) -o hashtable-test

libhashtable.so: MD5Digest.o HashTable.o
	$(CC) $(CFLAGS) -fPIC -shared $(OBJDIR)/MD5Digest.o $(OBJDIR)/HashTable.o -o libhashtable.so

MD5Digest.o: $(SRCDIR)/MD5Digest.cpp $(SRCDIR)/MD5Digest.h
	$(CC) $(CFLAGS) -fPIC -c $(SRCDIR)/MD5Digest.cpp -o $(OBJDIR)/MD5Digest.o
	
HashTable.o: $(SRCDIR)/HashTable.cpp $(SRCDIR)/HashTable.h
	$(CC) $(CFLAGS) -fPIC -c $(SRCDIR)/HashTable.cpp -o $(OBJDIR)/HashTable.o

clean:
	rm -f $(OBJDIR)/*.o
	rm -f hashtable-test
	rm -f libhashtable.so
