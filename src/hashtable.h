#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <openssl/md5.h>

#define HT_INITIAL_SIZE 64
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

// initializes the hash_table struct
void ht_init(hash_table *table);

// destroys the hash_table struct 
// and frees all relevant memory
void ht_destroy(hash_table *table);

// inserts the {key: value} pair into the hash table, 
// makes copies of both key and value
void ht_insert(hash_table *table, void *key, uint key_size, void *value, uint value_size);

// returns a pointer to the value with the matching key, 
// value_size is set to the size in bytes of the value
void* ht_get(hash_table *table, void *key, uint key_size, uint *value_size);

// removes the entry corresponding to the 
// specified key from the hash table
void ht_remove(hash_table *table, void *key, uint key_size);

// returns 1 if the hash_table contains the entry 
// specified by key, 0 otherwise
int ht_contains(hash_table *table, void *key, uint key_size);

// returns the number of valid entries 
// in the hash table
uint ht_size(hash_table *table);

// returns an array of keys
// sets key_count to the length of the array
void** ht_keys(hash_table *table, uint *key_count);

// removes all entries from the hash table
void ht_clear(hash_table *table);

// calulates the index in the hash table's internal array
// from the given key (used for debugging currently)
uint ht_index(hash_table *table, void *key, uint key_size);

// resizes the hash table's internal array
// and recomputes all of the keys
void ht_resize(hash_table *table, uint new_size);

//----------------------------------
// HashEntry functions
//----------------------------------

hash_entry *he_create(void *key, uint key_size, void *value, uint value_size);
void he_destroy(hash_entry *entry);

int he_key_compare(hash_entry *e1, hash_entry *e2);
void he_set_value(hash_entry *entry, void *value, uint value_size);

#endif
        
