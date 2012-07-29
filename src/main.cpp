#include "MD5Digest.h"
#include "CPPHashTable.h"
#include "dbg.h"
#include "timer.h"
#include "test.h"

#include <openssl/md5.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COLLISION_THRESHOLD 0.1

void testHash1();
void testHash2();
void testHash3();
void testMD5();

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    //testMD5();
    debug("testHash1()");
    testHash1();
    delim();
    debug("testHash2()");
    testHash2();
    delim();
    debug("testHash3()");
    testHash3();
    delim();

    return report_results();
}

void testHash1()
{
    debug("Inserting keys");
    const char *key1 = "key1";
    const char *key2 = "key2";
    HashTable<const char*, int> hash;
    
    hash.insert(key1, 5);
    hash.insert(key2, 6);

    debug("Checking HashTable::contains()");
    
    test(hash.contains(key1), "Key1 okay: %s", "Hashtable lost key1: %s", key1); 
    test(hash.contains(key2), "Key2 okay: %s", "Hashtable lost key2: %s", key2); 
    test(!hash.contains("invalidkey"), "Non-existent key \"%s\" was not in the hash", "Hashtable found non-existent key \"%s\"", "invalidkey"); 

    debug("Checking valid keys");
    int result1 = hash.value(key1);
    int result2 = hash.value(key2);

    test(result1 == 5, "Result 1 okay for key: \"%s\", value: %d", "Result 1 did not match for key: \"%s\" value: %d", key1, result1);
    test(result2 == 6, "Result 2 okay for key: \"%s\", value: %d", "Result 2 did not match for key: \"%s\" value: %d", key2, result2);

    debug("Hash collisions so far: %d", hash.collisions());
    
    debug("Stress testing hash table");

    hash.clear();
    int key_count = 1000;
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
    test(collision_ratio < COLLISION_THRESHOLD, "less than %.1f%% of inserts collided (%.2f%%)", "more than %.1f%% of inserts collided (%.2f%%)", COLLISION_THRESHOLD*100, collision_ratio*100)

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

    test(keys_okay, "HashTable::contains() found all of the keys", "HashTable::contains() lost some keys");
    test(values_okay, "HashTable::contains() found all of the values", "HashTable::contains() lost some values");

    for(int i = 0; i < key_count; i++)
    {
        delete[] keys[i];
    }

    delete[] keys;
    delete[] values;
    delete[] timings;
}


void testHash2()
{
    HashTable<int, const char*> hash;

    int k1 = 5;
    int k2 = 6;

    const char *v1 = "five";
    const char *v2 = "six";

    debug("Insert k1=%d v1=\"%s\"", k1, v1);
    hash.insert(k1, v1);

    debug("Insert k2=%d v2=\"%s\"", k2, v2);
    hash.insert(k2, v2);

    test(hash.contains(k1), "First key okay (%d, %s)", "First key borked (%d, %s)", k1, hash.value(k1));
    test(hash.contains(k2), "Second key okay (%d, %s)", "Second key borked (%d %s", k2, hash.value(k2));
}


class Classy
{
    public:
        bool operator==(const Classy& other);
        bool operator!=(const Classy& other);

        int int1;
        long long int int2;
        char *str1;
        char *str2;
};

bool Classy::operator==(const Classy& other)
{
    return (int1 == other.int1) && (int2 == other.int2) && (strcmp(str1, other.str1) == 0) && (strcmp(str2, other.str2) == 0);
}

bool Classy::operator!=(const Classy& other)
{
    return !((*this) == other);
}

void testHash3()
{
    Classy c;
    c.int1 = 42;
    c.int2 = 1337;
    c.str1 = (char*)"first string";
    c.str2 = (char*)"second string";
    int v = 10101;

    HashTable<Classy, int> hash1;
    HashTable<int, Classy> hash2;

    debug("Testing struct as key");
    hash1.insert(c, v);

    test(hash1.contains(c), "Hash 1 had key [%d, %lld, \"%s\", \"%s\"]", "Hash 2 did not have key [%d, %lld, \"%s\", \"%s\"]", c.int1, c.int2, c.str1, c.str2);
    test(hash1.value(c) == v, "Hash 1 had value %d", "Hash 1 did not have value %d", v);

    hash2.insert(v, c);

    test(hash2.contains(v), "Hash 2 had key %d", "Hash 2 did not have key %d", v);
    test(hash2.value(v) == c, "Hash 2 had value [%d, %lld, \"%s\", \"%s\"]", "Hash 2 did not have correct value [%d, %lld, \"%s\", \"%s\"]", c.int1, c.int2, c.str1, c.str2);
}


void testMD5()
{
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
}









