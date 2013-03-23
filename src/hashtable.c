// License: BSD 3-clause
// Author: Dane Larsen
// See LICENSE.txt for the complete license text

#include "hashtable.h"
#include "dbg.h"
#include "murmur.h"

#include <stdlib.h>

uint32_t global_seed = 2976579765;

struct hash_entry {
    void *key;
    void *value;
    size_t key_size;
    size_t value_size;
    struct hash_entry *next;
};

//----------------------------------
// HashEntry functions
//----------------------------------

hash_entry *he_create(int flags, void *key, size_t key_size, void *value, size_t value_size);
void he_destroy(int flags, hash_entry *entry);

int he_key_compare(hash_entry *e1, hash_entry *e2);
void he_set_value(int flags, hash_entry *entry, void *value, size_t value_size);

//-----------------------------------
// HashTable functions
//-----------------------------------

void ht_init(hash_table *table, ht_flags flags, double max_load_factor)
{
    table->array_size = HT_INITIAL_SIZE;
    table->array = malloc(table->array_size * sizeof(*(table->array)));
    check_mem(table->array);
    table->key_count = 0;
    table->collisions = 0;
    table->flags = flags;
    table->max_load_factor = max_load_factor;
    table->current_load_factor = 0.0;

    unsigned int i;
    for(i = 0; i < table->array_size; i++)
    {
        table->array[i] = NULL;
    }

    return;

error:
    log_err("Failed to allocate memory in ht_init()");
}

void ht_destroy(hash_table *table)
{
    unsigned int i;
    hash_entry *entry;
    hash_entry *tmp;

    if(table->array == NULL) {
        debug("ht_destroy got a bad table");
    }

    // crawl the entries and delete them
    for(i = 0; i < table->array_size; i++) {
        entry = table->array[i];

        while(entry != NULL) {
            tmp = entry->next;
            he_destroy(table->flags, entry);
            entry = tmp;
        }
    }

    table->array_size = 0;
    table->key_count = 0;
    table->collisions = 0;
    free(table->array);
    table->array = NULL;
}

void ht_insert(hash_table *table, void *key, size_t key_size, void *value, size_t value_size)
{
    hash_entry *entry = he_create(table->flags, key, key_size, value, value_size);
    ht_insert_he(table, entry);
}

// this was separated out of the regular ht_insert
// for ease of copying hash entries around
void ht_insert_he(hash_table *table, hash_entry *entry){
    hash_entry *tmp;
    unsigned int index;

    entry->next = NULL;
    index = ht_index(table, entry->key, entry->key_size);
    tmp = table->array[index];

    // if true, no collision
    if(tmp == NULL)
    {
        table->array[index] = entry;
        table->key_count++;
        return;
    }

    // walk down the chain until we either hit the end
    // or find an identical key (in which case we replace
    // the value)
    while(tmp->next != NULL)
    {
        if(he_key_compare(tmp, entry))
            break;
        else
            tmp = tmp->next;
    }

    if(he_key_compare(tmp, entry))
    {
        // if the keys are identical, throw away the old entry
        // and stick the new one into the table
        he_set_value(table->flags, tmp, entry->value, entry->value_size);
        he_destroy(table->flags, entry);
    }
    else
    {
        // else tack the new entry onto the end of the chain
        tmp->next = entry;
        table->collisions += 1;
        table->key_count ++;
        table->current_load_factor = (double)table->collisions / table->array_size;

        // double the size of the table if autoresize is on and the
        // load factor has gone too high
        if(!(table->flags & HT_NO_AUTORESIZE) && (table->current_load_factor > table->max_load_factor)) {
            ht_resize(table, table->array_size * 2);
            table->current_load_factor = (double)table->collisions / table->array_size;
        }
    }
}

void* ht_get(hash_table *table, void *key, size_t key_size, size_t *value_size)
{
    unsigned int index = ht_index(table, key, key_size);
    hash_entry *entry = table->array[index];
    hash_entry tmp;
    tmp.key = key;
    tmp.key_size = key_size;

    // once we have the right index, walk down the chain (if any)
    // until we find the right key or hit the end
    while(entry != NULL)
    {
        if(he_key_compare(entry, &tmp))
        {
            if(value_size != NULL)
                *value_size = entry->value_size;

            return entry->value;
        }
        else
        {
            entry = entry->next;
        }
    }

    return NULL;
}

void ht_remove(hash_table *table, void *key, size_t key_size)
{
    unsigned int index = ht_index(table, key, key_size);
    hash_entry *entry = table->array[index];
    hash_entry *prev = NULL;
    hash_entry tmp;
    tmp.key = key;
    tmp.key_size = key_size;

    // walk down the chain
    while(entry != NULL)
    {
        // if the key matches, take it out and connect its
        // parent and child in its place
        if(he_key_compare(entry, &tmp))
        {
            if(prev == NULL)
                table->array[index] = entry->next;
            else
                prev->next = entry->next;

            table->key_count--;
            if(prev != NULL)
              table->collisions--;
            he_destroy(table->flags, entry);
            return;
        }
        else
        {
            prev = entry;
            entry = entry->next;
        }
    }
}

int ht_contains(hash_table *table, void *key, size_t key_size)
{
    unsigned int index = ht_index(table, key, key_size);
    hash_entry *entry = table->array[index];

    hash_entry tmp;
    tmp.key = key;
    tmp.key_size = key_size;

    // walk down the chain, compare keys
    while(entry != NULL)
    {
        if(he_key_compare(entry, &tmp))
            return 1;
        else
            entry = entry->next;
    }

    return 0;
}

unsigned int ht_size(hash_table *table)
{
    return table->key_count;
}

void** ht_keys(hash_table *table, unsigned int *key_count)
{
    void **ret;

    if(table->key_count == 0){
      *key_count = 0;
      return NULL;
    }

    // array of pointers to keys
    ret = malloc(table->key_count * sizeof(void *));
    check_mem(ret);
    *key_count = 0;

    unsigned int i;
    hash_entry *tmp;

    // loop over all of the chains, walk the chains,
    // add each entry to the array of keys
    for(i = 0; i < table->array_size; i++)
    {
        tmp = table->array[i];

        while(tmp != NULL)
        {
            ret[*key_count]=tmp->key;
            *key_count += 1;
            tmp = tmp->next;
            // sanity check, should never actually happen
            check_debug(*key_count >= table->key_count, "ht_keys: too many keys");
        }
    }

    return ret;
error:
    return NULL;
}

void ht_clear(hash_table *table)
{
    ht_destroy(table);
    ht_init(table, table->flags, table->max_load_factor);
}

unsigned int ht_index(hash_table *table, void *key, size_t key_size)
{
    uint32_t index;
    // 32 bits of murmur seems to fare pretty well
    MurmurHash3_x86_32(key, key_size, global_seed, &index);
    index %= table->array_size;
    return index;
}

// new_size can be smaller than current size (downsizing allowed)
void ht_resize(hash_table *table, unsigned int new_size)
{
    hash_table new_table;

    debug("ht_resize(old=%d, new=%d)",table->array_size,new_size);
    new_table.array_size = new_size;
    new_table.array = malloc(new_size * sizeof(hash_entry*));
    new_table.key_count = 0;
    new_table.collisions = 0;
    new_table.flags = table->flags;
    new_table.max_load_factor = table->max_load_factor;

    unsigned int i;
    for(i = 0; i < new_table.array_size; i++)
    {
        new_table.array[i] = NULL;
    }

    hash_entry *entry;
    hash_entry *next;
    for(i = 0; i < table->array_size; i++)
    {
        entry = table->array[i];
        while(entry != NULL)
        {
            next = entry->next;
            ht_insert_he(&new_table, entry);
            entry = next;
        }
        table->array[i]=NULL;
    }

    ht_destroy(table);

    table->array_size = new_table.array_size;
    table->array = new_table.array;
    table->key_count = new_table.key_count;
    table->collisions = new_table.collisions;

}

void ht_set_seed(uint32_t seed){
    global_seed = seed;
}

//---------------------------------
// hash_entry functions
//---------------------------------

hash_entry *he_create(int flags, void *key, size_t key_size, void *value, size_t value_size)
{
    hash_entry *entry = malloc(sizeof(*entry));
    check_mem(entry);

    entry->key_size = key_size;
    if (flags & HT_KEY_CONST){
        entry->key = key;
    } else {
        entry->key = malloc(key_size);
        check_mem(entry->key);
        memcpy(entry->key, key, key_size);
    }

    entry->value_size = value_size;
    if (flags & HT_VALUE_CONST){
        entry->value = value;
    } else {
        entry->value = malloc(value_size);
        check_mem(entry->value);
        memcpy(entry->value, value, value_size);
    }

    entry->next = NULL;

    return entry;

error:
    log_err("Failed to create hash_entry");

    if(entry) {
        if(entry->key)
            free(entry->key);

        if(entry->value)
            free(entry->value);

        free(entry);
    }

    return NULL;
}

void he_destroy(int flags, hash_entry *entry)
{
    if (!(flags & HT_KEY_CONST))
        free(entry->key);
    if (!(flags & HT_VALUE_CONST))
        free(entry->value);
    free(entry);
}

int he_key_compare(hash_entry *e1, hash_entry *e2)
{
    char *k1 = e1->key;
    char *k2 = e2->key;

    if(e1->key_size != e2->key_size)
        return 0;

    return (memcmp(k1,k2,e1->key_size) == 0);
}

void he_set_value(int flags, hash_entry *entry, void *value, size_t value_size)
{
    if (!(flags & HT_VALUE_CONST)){
        if(entry->value)
            free(entry->value);

        entry->value = malloc(value_size);
        check_mem(entry->value);
        memcpy(entry->value, value, value_size);
    } else {
        entry->value = value;
    }
    entry->value_size = value_size;

    return;

error:
    log_err("Failed to set entry value");
}



