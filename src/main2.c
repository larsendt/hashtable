#include "hashtable.h"
#include "dbg.h"
#include "test.h"

#include <string.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    hash_table ht;
    ht_init(&ht);

    char *s1 = (char*)"harp darp";
    char *s2 = (char*)"NO";
    char *s3 = (char*)"ODIN SAYS";

    ht_insert(&ht, s1, strlen(s1)+1, s2, strlen(s2)+1);

    int contains = ht_contains(&ht, s1, strlen(s1)+1);
    test(contains, "Hash table had key \"%s\"", "Hash table did not have key \"%s\"", s1);
    
    unsigned int value_size;
    char *got = ht_get(&ht, s1, strlen(s1)+1, &value_size);
    
    debug("Value size: %u", value_size);
    debug("Got: {\"%s\": \"%s\"}", s1, got);
   
    test(value_size == strlen(s2)+1, 
            "Value size was %u (desired %lu)", 
            "Value size was %u, should be %lu", 
            value_size, strlen(s2)+1);

    debug("Replacing {\"%s\": \"%s\"} with {\"%s\": \"%s\"}", s1, s2, s1, s3);
    ht_insert(&ht, s1, strlen(s1)+1, s3, strlen(s3)+1);

    got = ht_get(&ht, s1, strlen(s1)+1, &value_size);
    
    debug("Value size: %u", value_size);
    debug("Got: {\"%s\": \"%s\"}", s1, got);
   
    test(value_size == strlen(s3)+1, 
            "Value size was %u (desired %lu)", 
            "Value size was %u, should be %lu", 
            value_size, strlen(s3)+1);

    debug("Removing entry with key \"%s\"", s1);
    ht_remove(&ht, s1, strlen(s1)+1);

    contains = ht_contains(&ht, s1, strlen(s1)+1);
    test(!contains, "Hash table no longer has key \"%s\"", "Hash table still has key \"%s\"", s1);


    debug("Stress test");
    int key_count = 100000;
    int i;

    srand(time(NULL));

    for(i = 0; i < key_count; i++)
    {
        int k = rand();
        int v = rand();

        ht_insert(&ht, &k, sizeof(k), &v, sizeof(v));
    }   

    ht_destroy(&ht);

    return report_results();
}





