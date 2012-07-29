#include "hashtable.h"

#include "dbg.h"
#include <stdlib.h>

void ht_init(hash_table *table)
{
    table->array_size = HT_INITIAL_SIZE;
    table->array = malloc(table->array_size * sizeof(*(table->array)));
    check_mem(table->array);
    table->key_count = 0;
    table->collisions = 0;

    for(uint i = 0; i < table->array_size; i++)
    {
        table->array[i] = NULL;
    }

    return;

error:
    log_err("Failed to allocate memory in ht_init()");
}

void ht_destroy(hash_table *table)
{
    uint i;
    hash_entry *entry;
    hash_entry *tmp;

    for(i = 0; i < table->array_size; i++)
    {
        entry = table->array[i];

        while(entry != NULL)
        {
            tmp = entry->next;
            he_destroy(entry);
            entry = tmp;
        }
    }
    
    table->array_size = 0;
    table->key_count = 0;
    free(table->array);
}

void ht_insert(hash_table *table, void *key, uint key_size, void *value, uint value_size)
{
    hash_entry *entry = he_create(key, key_size, value, value_size); 
    hash_entry *tmp;
    uint index;

    index = ht_index(table, key, key_size);
    tmp = table->array[index];

    if(tmp == NULL)
    {
        table->array[index] = entry;
        return;
    }

    while(tmp->next != NULL)
    {
        if(he_key_compare(tmp, entry))
            break;
        else
            tmp = tmp->next;
    }

    if(he_key_compare(tmp, entry))
    {
        he_set_value(tmp, entry->value, entry->value_size);
        he_destroy(entry);
    }
    else
    {
        tmp->next = entry;
        table->collisions += 1;
    }
}

void* ht_get(hash_table *table, void *key, uint key_size, uint *value_size)
{
    uint index = ht_index(table, key, key_size);
    hash_entry *entry = table->array[index];
    hash_entry tmp;
    tmp.key = key;
    tmp.key_size = key_size;

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

void ht_remove(hash_table *table, void *key, uint key_size)
{
    uint index = ht_index(table, key, key_size);
    hash_entry *entry = table->array[index];

    hash_entry tmp;
    tmp.key = key;
    tmp.key_size = key_size;

    while(entry != NULL)
    {
        if(he_key_compare(entry, &tmp))
        {
            if(entry->prev == NULL)
                table->array[index] = entry->next;
            else
                entry->prev->next = entry->next;
            
            he_destroy(entry);
            return;
        }
        else
        {
            entry = entry->next;
        }
    }
}

int ht_contains(hash_table *table, void *key, uint key_size)
{
    uint index = ht_index(table, key, key_size);
    hash_entry *entry = table->array[index];
    
    hash_entry tmp;
    tmp.key = key;
    tmp.key_size = key_size;

    while(entry != NULL)
    {
        if(he_key_compare(entry, &tmp))
            return 1;
        else
            entry = entry->next;
    }

    return 0;
}

uint ht_size(hash_table *table)
{
    return table->key_count;
}

void** ht_keys(hash_table *table)
{
    log_err("NOT IMPLEMENTED");
    return NULL;
}

void ht_clear(hash_table *table)
{
    log_err("NOT IMPLEMENTED");
}

uint ht_index(hash_table *table, void *key, uint key_size)
{
    unsigned char *md5_hash = MD5(key, key_size, NULL);
    uint index;
    memcpy(&index, md5_hash, sizeof(index));
    index %= table->array_size;
    return index;
}

//---------------------------------
// hash_entry functions
//---------------------------------

hash_entry *he_create(void *key, uint key_size, void *value, uint value_size)
{
    hash_entry *entry = malloc(sizeof(*entry));
    check_mem(entry);

    entry->key_size = key_size;
    entry->key = malloc(key_size);
    check_mem(entry->key);
    memcpy(entry->key, key, key_size);

    entry->value_size = value_size;
    entry->value = malloc(value_size);
    check_mem(entry->value);
    memcpy(entry->value, value, value_size);

    entry->next = NULL;
    entry->prev = NULL;

    return entry;

error:
    log_err("Failed to create hash_entry");

    if(entry)
        free(entry);

    if(entry->key)
        free(entry->key);

    if(entry->value)
        free(entry->value);

    return NULL;
}

void he_destroy(hash_entry *entry)
{
    free(entry->key);
    free(entry->value);
    free(entry);
}

int he_key_compare(hash_entry *e1, hash_entry *e2)
{
    uint i;
    char *k1 = e1->key;
    char *k2 = e2->key;

    if(e1->key_size != e2->key_size)
        return 0;

    for(i = 0; i < e1->key_size; i++)
    {
        if(k1[i] != k2[i])
            return 0;
    }

    return 1;
}

void he_set_value(hash_entry *entry, void *value, uint value_size)
{
    if(entry->value)
        free(entry->value);

    entry->value = malloc(value_size);
    check_mem(entry->value);
    memcpy(entry->value, value, value_size);

    entry->value_size = value_size;

    return;

error:
    log_err("Failed to set entry value");
}



