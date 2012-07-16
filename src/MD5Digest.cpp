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


void print_mdbuf(uchar *mdbuf, uint mdsize);

MD5Digest::MD5Digest(const uchar *data, uint64_t size)
{
    debug("Message size: %lu 0x%lx", size, size);
    int a = 0x67452301;
    int b = 0xefcdab89;
    int c = 0x98badcfe;
    int d = 0x10325476;
   
    uint T[64] = {0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
                  0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
                  0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
                  0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
                  0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
                  0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
                  0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
                  0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
                  0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
                  0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
                  0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
                  0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
                  0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
                  0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
                  0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
                  0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };

    (void) T;

    uint n = ((size + 9) / 64) + 1;
    uint mdsize = n * 64;;
    uchar *mdbuf = new uchar[mdsize];

    // copy the message into the data buffer
    memcpy(mdbuf, data, size);

    // set the "1" bit
    mdbuf[size] = 0x80;

    // pad with zeroes
    for(uint i = size+1; i < mdsize-8; i++)
    {
        mdbuf[i] = 0;  
    } 

    // copy the 64-bit size variable into the last 8 bytes
    // of the array, in little-endian order 
    memcpy(mdbuf + (mdsize-8), &size, sizeof(size));

    print_mdbuf(mdbuf, mdsize);

    uint32_t x[16];
    int aa, bb, cc, dd;

    for(uint i = 0; i < n; i++)
    {
        memcpy(x, mdbuf + (i*64), 64);

        aa = a;
        bb = b;
        cc = c;
        dd = d;



    } 

    delete[] mdbuf;
}

void print_mdbuf(uchar *mdbuf, uint mdsize)
{
#ifdef DEBUG

    fprintf(stderr, "\n");
    debug("mdbuf:");


    for(uint64_t i = 0; i < mdsize-8; i++)
    {
        if(i % 8 == 0)
            fprintf(stderr, "\n  ");

        if(mdbuf[i] <= 126 && mdbuf[i] >= 32)
            fprintf(stderr, CYAN(" %c "), mdbuf[i]);
        else if(mdbuf[i] == 10)
            fprintf(stderr, CYAN("\\n "));
        else if(mdbuf[i] == 9)
            fprintf(stderr, CYAN("\\t "));
        else if(mdbuf[i] == 128)
            fprintf(stderr, RED("%02x "), mdbuf[i]);
        else
            fprintf(stderr, "%02x ", mdbuf[i]);
    }

    fprintf(stderr, "\n  ");

    for(uint i = mdsize-8; i < mdsize; i++)
    {
        fprintf(stderr, GREEN("%02x "), mdbuf[i]);
    }
    
    fprintf(stderr, NORM("\n\n"));
#endif //DEBUG
}



