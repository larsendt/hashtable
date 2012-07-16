#include "MD5Digest.h"
#include "HashTable.h"
#include "dbg.h"
#include "timer.h"

#include <openssl/md5.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
/*
    const char *str = "this is a test message that will be hashed. ";

    MD5Digest d((const unsigned char*)str, strlen(str));

    fprintf(stderr, "     me: ");
    for(int i = 0; i < 16; i++)
    {
        fprintf(stderr, "%02x", d.bytes()[i]);
    }
    fprintf(stderr, "\n");

    unsigned char *md5_hash = MD5((const unsigned char*)str, strlen(str), NULL);
    fprintf(stderr, "openssl: ");
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
        fprintf(stderr, "%02x", md5_hash[i]);
    }
    fprintf(stderr, "\n");

    bool equal = true;
    for(int i = 0; i < 16; i++)
    {
        if(d.bytes()[i] != md5_hash[i])
           equal = false;
    }

    if(equal)
       debug("i win");
    else
       debug("openssl wins");

    delim();*/
    debug("Hashtable test");

    delim();
    debug("Inserting keys");
    const char *key1 = "key1";
    const char *key2 = "key2";
    HashTable<const char*, int> hash;
    
    hash.insert(key1, 5);
    hash.insert(key2, 6);

    delim();
    debug("Checking HashTable::contains()");
    if(hash.contains(key1))
        debug("Key1 okay");
    else
        debug("ERROR: Hashtable lost key1");

    if(hash.contains(key2))
        debug("Key2 okay");
    else
        debug("ERROR: Hashtable lost key2");

    if(hash.contains("invalidKey"))
        debug("ERROR: Hashtable found non-existent key");
    else
        debug("Hashtable didn't have non-existent key (that's a good thing)");

    delim();
    debug("Checking valid keys");
    int result1 = hash.value(key1);
    int result2 = hash.value(key2);

    if(result1 == 5)
        debug("Result 1 okay for key: \"%s\" value: %d", key1, result1);
    else
        debug("ERROR: Result 1 did not match for key: \"%s\" value %d", key1, result1);

    if(result2 == 6)
        debug("Result 2 okay for key: \"%s\" value: %d", key2, result2);
    else
        debug("ERROR: Result 2 did not match for key: \"%s\" value %d", key2, result2);

    delim();
    debug("Checking invalid keys");
    const char *invalidKey = "invalidKey";
    int invalidResult = hash.value(invalidKey);

    debug("Invalid result was: %d", invalidResult);
    
    delim();
    debug("Hash collisions so far: %d", hash.collisions());

    delim();
    debug("Stress testing hash table");

    hash.clear();
    int key_count = 10000;
    char **keys = new char*[key_count];
    int *values = new int[key_count];
    long long int *timings = new long long int[key_count];

    srand(time(NULL));

    long long int c1;
    long long int c2;

    debug("Inserting %d k/v pairs", key_count);
    for(int i = 0; i < key_count; i++)
    {
        int v = rand();
        values[i] = v;
        keys[i] = new char[33];
        sprintf(keys[i], "%x", v);
        
        getcycles(&c1);
        hash.insert(keys[i], v);
        getcycles(&c2);

        timings[i] = c2 - c1;
    }

    double collision_ratio = (double)hash.collisions() / key_count;
    debug("Hash collisions: %d", hash.collisions());
    debug("%.4f%% of insertions collided", collision_ratio*100);
   
    long long int sum = 0;
    long long int avg;
    for(int i = 0; i < key_count; i++)
    {
        sum += timings[i];
    }

    avg = sum / key_count;
    debug("Average insertion took %lld cycles", avg);

    debug("Checking inserted k/v pairs");

    bool keys_okay = true;
    bool values_okay = true;
    for(int i = 0; i < key_count; i++)
    {
        if(!hash.contains(keys[i]))
        {
            debug("Hash lost key: %s\n", keys[i]);
            keys_okay = false;
        }
    
        if(hash.value(keys[i]) != values[i])
        {
            debug("Hash lost key: %s value %d", keys[i], values[i]);
            values_okay = false;
        }
    }

    if(keys_okay)
        debug("HashTable::contains() found all of the keys");
    else
        debug("ERROR: HashTable::contains() lost some keys");

    if(values_okay)
        debug("HashTable::value() had all of the values");
    else
        debug("ERROR: HashTable::value() lost some values");

    for(int i = 0; i < key_count; i++)
    {
       printf("%d %lld\n", i, timings[i]);
    } 

    for(int i = 0; i < key_count; i++)
    {
        delete[] keys[i];
    }

    delete[] keys;
    delete[] values;
    delete[] timings;
}












