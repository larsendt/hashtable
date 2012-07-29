#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <openssl/md5.h>

#define HT_INITIAL_SIZE 128
typedef unsigned int uint;

typedef struct hash_entry {
    void *key;
    void *value;
    uint key_size;
    uint value_size;
    struct hash_entry *next;
    struct hash_entry *prev;
} hash_entry;

typedef struct hash_table {
    uint key_count;
    uint array_size;
    hash_entry **array;
    uint collisions;
} hash_table;

//----------------------------------
// HashTable functions
//----------------------------------

void ht_init(hash_table *table);
void ht_destroy(hash_table *table);

void ht_insert(hash_table *table, void *key, uint key_size, void *value, uint value_size);
void* ht_get(hash_table *table, void *key, uint key_size, uint *value_size);
void ht_remove(hash_table *table, void *key, uint key_size);
int ht_contains(hash_table *table, void *key, uint key_size);

uint ht_size(hash_table *table);
void** ht_keys(hash_table *table);
void ht_clear(hash_table *table);

uint ht_index(hash_table *table, void *key, uint key_size);

//----------------------------------
// HashEntry functions
//----------------------------------

hash_entry *he_create(void *key, uint key_size, void *value, uint value_size);
void he_destroy(hash_entry *entry);

int he_key_compare(hash_entry *e1, hash_entry *e2);
void he_set_value(hash_entry *entry, void *value, uint value_size);

#endif
        
