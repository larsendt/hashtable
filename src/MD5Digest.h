#ifndef MD5_DIGEST_H
#define MD5_DIGEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

class MD5Digest
{
    public:
        MD5Digest(const unsigned char* data, uint64_t size);
        uint8_t* bytes() { return m_bytes; }        
        
    private:
        uint8_t m_bytes[16];

};

#endif
