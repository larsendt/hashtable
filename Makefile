SRCDIR	= src
OBJDIR	= obj
CC 		= gcc
CFLAGS	= -Wall -Wextra -g -DDEBUG -DTEST -pg
LFLAGS	= -lcrypto -lrt -pg
VERSION	= 0.1

all: hashtable-test

hashtable-test: $(SRCDIR)/main.c $(SRCDIR)/hashtable.h $(SRCDIR)/hashtable.c
	$(CC) $(SRCDIR)/main.c $(SRCDIR)/hashtable.c $(LFLAGS) $(CFLAGS) -o hashtable-test

clean:
	rm -f $(OBJDIR)/*.o
	rm -f hashtable-test
	rm -f libhashtable.a
