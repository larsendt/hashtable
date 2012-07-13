#include "MD5Digest.h"
#include "dbg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char uchar;
typedef unsigned int uint;

#define F(X, Y, Z) (X & Y) | ((~X) & Z)
#define G(X, Y, Z) (X & Z) | (Y & (~Z))
#define H(X, Y, Z) X ^ Y ^ Z
#define I(X, Y, Z) Y ^ (X | (~Z))


uint get_fullsize(uint64_t size);
void print_fulldata(uchar *fulldata, uint fullsize);

MD5Digest::MD5Digest(const uchar *data, uint64_t size)
{
    debug("Size: %d", size);

    int a = 0x67452301;
    int b = 0xefcdab89;
    int c = 0x98badcfe;
    int d = 0x10325476;

    uint fullsize = get_fullsize(size);
    uchar *fulldata = new uchar[fullsize];

    print_fulldata(fulldata, fullsize);

    delete[] fulldata;
}


uint get_fullsize(uint64_t size)
{
    uint n = (size + 9) / 64; 
    uint fullsize = n * 64;
    return fullsize;
}

void print_fulldata(uchar *fulldata, uint fullsize)
{
    debug("Fulldata:");
}
