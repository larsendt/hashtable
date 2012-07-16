#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "dbg.h"

template <class K, class V>
class HashTable
{
    public:
        HashTable();
};

template <class K, class V>
HashTable<K, V>::HashTable()
{
    debug("Key size: %d\n", sizeof(K));
    debug("Value size: %d\n", sizeof(V));
}

#endif
        
