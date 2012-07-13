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
    int a = 0x67452301;
    int b = 0xefcdab89;
    int c = 0x98badcfe;
    int d = 0x10325476;

    uint fullsize = get_fullsize(size);
    uchar *fulldata = new uchar[fullsize];

    // copy the message into the data buffer
    memcpy(fulldata, data, size);

    // set the "1" bit
    fulldata[size] = 0x80;

    // pad with zeroes
    for(uint i = size+1; i < fullsize-8; i++)
    {
        fulldata[i] = 0;  
    } 

    // copy the 
    memcpy(fulldata + (fullsize-8), &size, sizeof(size));

    print_fulldata(fulldata, fullsize);

    delete[] fulldata;
}


uint get_fullsize(uint64_t size)
{
    uint n = ((size + 9) / 64) + 1; 
    uint fullsize = n * 64;
    return fullsize;
}

void print_fulldata(uchar *fulldata, uint fullsize)
{
#ifdef DEBUG

    fprintf(stderr, "\n");
    debug("Fulldata:");


    for(uint64_t i = 0; i < fullsize-8; i++)
    {
        if(i % 8 == 0)
            fprintf(stderr, "\n  ");

        if(fulldata[i] <= 126 && fulldata[i] >= 32)
            fprintf(stderr, CYAN(" %c "), fulldata[i]);
        else if(fulldata[i] == 10)
            fprintf(stderr, CYAN("\\n "));
        else if(fulldata[i] == 9)
            fprintf(stderr, CYAN("\\t "));
        else if(fulldata[i] == 128)
            fprintf(stderr, RED("%02x "), fulldata[i]);
        else
            fprintf(stderr, "%02x ", fulldata[i]);
    }

    fprintf(stderr, "\n  ");

    for(uint i = fullsize-8; i < fullsize; i++)
    {
        fprintf(stderr, GREEN("%02x "), fulldata[i]);
    }
    
    fprintf(stderr, NORM("\n\n"));
#endif //DEBUG
}



