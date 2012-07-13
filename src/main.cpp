#include "MD5Digest.h"
#include "dbg.h"

#include <openssl/md5.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    const char *str = "this is a test message that will be hashed. "
                      "I'm attempting to make it longer. "
                      "Let's see if we can make the size bigger than "
                      "the available space. What happens if we put "
                      "a newline character in here? \n And another "
                      "\n More text moar text MOAR TEXT WOAH THIS IS "
                      "GETTING MASSIVE.";

    MD5Digest d((const unsigned char*)str, strlen(str));

    printf("     me: ");
    for(int i = 0; i < 16; i++)
    {
        printf("%02x", d.bytes()[i]);
    }
    printf("\n");

    unsigned char *md5_hash = MD5((const unsigned char*)str, strlen(str), NULL);
    printf("openssl: ");
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
        printf("%02x", md5_hash[i]);
    }
    printf("\n");

    bool equal = true;
    for(int i = 0; i < 16; i++)
    {
        if(d.bytes()[i] != md5_hash[i])
           equal = false;
    }

    if(equal)
       printf("i win\n");
    else
       printf("openssl wins\n");

}
