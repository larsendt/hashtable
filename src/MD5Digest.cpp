#include "MD5Digest.h"
#include "dbg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t leftrotate(uint32_t x, uint32_t c);
typedef unsigned char uchar;
typedef unsigned int uint;

MD5Digest::MD5Digest(const uchar *data, uint64_t size)
{
    const uint32_t r[64] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                             5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
                             4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                             6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };
                            
    const uint32_t k[64] = { 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
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

    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xefcdab89;
    uint32_t h2 = 0x98badcfe;
    uint32_t h3 = 0x10325476;

    uint64_t n = ((size + 9) / 64) + 1;
    uint64_t fullsize = n * 64;

    uchar *fulldata = new uchar[fullsize];
    
    memcpy(fulldata, data, size);
    fulldata[size] = 0x80;
    
    for(uint i = size+1; i < (fullsize-8); i++)
    {
        fulldata[i] = 0x0;
    }
    
    memcpy(fulldata + (fullsize-8), &size, sizeof(size));

    /* debug block */
    for(uint64_t i = 0; i < fullsize-8; i++)
    {
        if(i % 8 == 0)
            printf("\n  ");

        if(fulldata[i] <= 126 && fulldata[i] >= 32)
            printf(" %c ", fulldata[i]);
        else
            printf("%02x ", fulldata[i]);
    }

    printf("\n  ");

    for(uint i = fullsize-8; i < fullsize; i++)
    {
        printf("%02x ", fulldata[i]);
    }

    printf("\n\n");
    /* end debug */

    uint32_t w[16];

    for(uint i = 0; i < n; i++)
    {   
        memcpy(w, fulldata + (i*64), 64);
        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        uint32_t f, g, tmp;

        for(int j = 0; j < 64; j++)
        {
            if(j >= 0 && j <= 15)
            {
                f = (b & c) | ((~b) & d);
                g = j;
            }
            else if(j >= 16 && j <= 31)
            {
                f = (d & b) | ((~d) & c);
                g = ((5 * j) + 1) % 16;
            }
            else if(j >= 32 && j <= 47)
            {
                f = b ^ c ^ d;
                g = ((3 * j) + 5) % 16;
            }
            else if(j >= 48 && j <= 63)
            {
                f = c ^ (b | (~d));
                g = (7 * j) % 16;
            }
            
            tmp = d;
            d = c;
            c = b;
            b = b + leftrotate((a + f + k[j] + w[g]), r[j]);
            a = tmp;
        }

        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
    }

    memcpy(m_bytes, &h0, 4);
    memcpy(m_bytes+4, &h1, 4);
    memcpy(m_bytes+8, &h2, 4);
    memcpy(m_bytes+12, &h3, 4);
    
    delete[] fulldata;
}


uint32_t leftrotate(uint32_t x, uint32_t c)
{
    return (x << c) | (x >> (32 - c));
}

