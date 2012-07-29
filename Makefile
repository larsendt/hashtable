SRCDIR	= src
OBJDIR	= obj
CC 		= clang
CPP		= clang++
CFLAGS	= -Wall -Wextra -g -DDEBUG -DTEST 
LFLAGS	= -L. -lhashtable -lcpphashtable -lcrypto 
VERSION	= 0.1

all: libhashtable.so chashtable-test cpphashtable-test

cpphashtable-test: $(SRCDIR)/main.cpp $(SRCDIR)/MD5Digest.h $(SRCDIR)/CPPHashTable.h libcpphashtable.so 
	$(CPP) $(SRCDIR)/main.cpp $(LFLAGS) $(CFLAGS) -o cpphashtable-test

chashtable-test: $(SRCDIR)/main2.c $(SRCDIR)/hashtable.h libhashtable.so
	$(CC) $(SRCDIR)/main2.c $(LFLAGS) $(CFLAGS) -o chashtable-test

libcpphashtable.so: MD5Digest.o CPPHashTable.o 
	$(CPP) $(CFLAGS) -fPIC -shared $(OBJDIR)/MD5Digest.o $(OBJDIR)/CPPHashTable.o -o libcpphashtable.so

MD5Digest.o: $(SRCDIR)/MD5Digest.cpp $(SRCDIR)/MD5Digest.h
	$(CPP) $(CFLAGS) -fPIC -c $(SRCDIR)/MD5Digest.cpp -o $(OBJDIR)/MD5Digest.o
	
CPPHashTable.o: $(SRCDIR)/CPPHashTable.cpp $(SRCDIR)/CPPHashTable.h
	$(CPP) $(CFLAGS) -fPIC -c $(SRCDIR)/CPPHashTable.cpp -o $(OBJDIR)/CPPHashTable.o

libhashtable.so: hashtable.o 
	$(CC) $(CFLAGS) -fPIC -shared $(OBJDIR)/hashtable.o -o libhashtable.so

hashtable.o:
	$(CC) $(CFLAGS) -fPIC -c $(SRCDIR)/hashtable.c -o $(OBJDIR)/hashtable.o

clean:
	rm -f $(OBJDIR)/*.o
	rm -f hashtable-test
	rm -f libhashtable.so
