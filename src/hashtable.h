// License: BSD 3-clause
// Author: Dane Larsen
// See LICENSE.txt for the complete license text

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdint.h>
#include <stddef.h>

#define HT_INITIAL_SIZE 64

typedef struct hash_entry {
    void *key;
    void *value;
    size_t key_size;
    size_t value_size;
    struct hash_entry *next;
} hash_entry;

typedef struct hash_table {
    unsigned int key_count;
    unsigned int array_size;
    hash_entry **array;
    unsigned int collisions;
    int flags;
} hash_table;

// HT flags
#define HT_NONE 0
#define HT_KEY_CONST 1
#define HT_VALUE_CONST 2

//----------------------------------
// HashTable functions
//----------------------------------

// initializes the hash_table struct
void ht_init(hash_table *table, int flags);

// destroys the hash_table struct 
// and frees all relevant memory
void ht_destroy(hash_table *table);

// inserts the {key: value} pair into the hash table, 
// makes copies of both key and value
void ht_insert(hash_table *table, void *key, size_t key_size, void *value, size_t value_size);

// inserts an existing hash entry into the hash table
void ht_insert_he(hash_table *table, hash_entry *entry);

// returns a pointer to the value with the matching key, 
// value_size is set to the size in bytes of the value
void* ht_get(hash_table *table, void *key, size_t key_size, size_t *value_size);

// removes the entry corresponding to the 
// specified key from the hash table
void ht_remove(hash_table *table, void *key, size_t key_size);

// returns 1 if the hash_table contains the entry 
// specified by key, 0 otherwise
int ht_contains(hash_table *table, void *key, size_t key_size);

// returns the number of valid entries 
// in the hash table
unsigned int ht_size(hash_table *table);

// returns an array of keys
// sets key_count to the length of the array
void** ht_keys(hash_table *table, unsigned int *key_count);

// removes all entries from the hash table
void ht_clear(hash_table *table);

// calulates the index in the hash table's internal array
// from the given key (used for debugging currently)
unsigned int ht_index(hash_table *table, void *key, size_t key_size);

// resizes the hash table's internal array
// and recomputes all of the keys
void ht_resize(hash_table *table, unsigned int new_size);

// sets the global security seed to be used in hash function
void ht_set_seed(uint32_t seed);

//----------------------------------
// HashEntry functions
//----------------------------------

hash_entry *he_create(int flags, void *key, size_t key_size, void *value, size_t value_size);
void he_destroy(int flags, hash_entry *entry);

int he_key_compare(hash_entry *e1, hash_entry *e2);
void he_set_value(int flags, hash_entry *entry, void *value, size_t value_size);

#endif
        
