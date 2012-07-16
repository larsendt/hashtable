#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "dbg.h"
#include <openssl/md5.h>
#include <cstdlib>
#include <stdint.h>

template <class K, class V>
class HashEntry;

//-----------------------------------
// HashTable Definition
//-----------------------------------

template <class K, class V>
class HashTable
{
    public:
        HashTable();
        ~HashTable();

        void insert(K key, V value);
        V value(K key);
        bool contains(K key);
        void clear();

        uint64_t hashIndex(K key);
        int collisions();

    private:
        HashEntry<K, V> **m_table;
        uint64_t m_size;    
        int m_coll;    
};

//-------------------------------------
// HashTable Members
//-------------------------------------

template <class K, class V>
HashTable<K, V>::HashTable()
{
    m_size = 128;
    m_coll = 0;
    m_table = new HashEntry<K, V>*[m_size];

    for(uint64_t i = 0; i < m_size; i++)
    {
        m_table[i] = NULL;
    } 
}

template <class K, class V>
HashTable<K, V>::~HashTable()
{
    HashEntry<K, V> *entry;
    HashEntry<K, V> *next;
    for(uint64_t i = 0; i < m_size; i++)
    {
        entry = m_table[i];
        
        while(entry != NULL)
        {
            next = entry->next();
            delete entry;
            entry = next;
        }
    }

    delete[] m_table;
    m_size = 0;
}

template<class K, class V>
void HashTable<K, V>::insert(K key, V value)
{
    uint64_t index = hashIndex(key);
    HashEntry<K, V> *entry = new HashEntry<K, V>(key, value);
    HashEntry<K, V> *parent = m_table[index];
    
    if(parent == NULL)
    {
        m_table[index] = entry;
        return;
    }
    else
    {
        m_coll++;
    }

    while(parent->next() != NULL)
    {
        parent = parent->next();
    }

    parent->setNext(entry);
}

template <class K, class V>
V HashTable<K, V>::value(K key)
{
    uint64_t index = hashIndex(key);
    HashEntry<K, V> *entry = m_table[index];

    if(entry == NULL)
    {
        debug("HashTable: No corresponding value for index %lu", index);
        return V();
    }

    while(entry->key() != key && entry->next() != NULL)
    {
        entry = entry->next();
    }

    if(entry->key() == key)
    {
        return entry->value();
    }
    else
    {
        debug("HashTable: Found entry for index %lu but no matching key", index);
        return V();
    }
}

template <class K, class V>
bool HashTable<K, V>::contains(K key)
{
    uint64_t index = hashIndex(key);
    HashEntry<K, V> *entry = m_table[index];

    if(entry == NULL)
        return false;

    while(entry->key() != key && entry->next() != NULL)
    {
        entry = entry->next();
    }

    return (entry->key() == key);
}

template <class K, class V>
uint64_t HashTable<K, V>::hashIndex(K key)
{
    //TODO is this stupid?
    unsigned char* md5_hash = MD5((const unsigned char*)key, sizeof(K), NULL);

    //TODO is this also really stupid?
    uint64_t index;
    memcpy(&index, md5_hash, sizeof(index));
    index %= m_size;
    return index;
}

template <class K, class V>
int HashTable<K, V>::collisions()
{
    return m_coll;
}

template <class K, class V>
void HashTable<K, V>::clear()
{
    m_coll = 0;
    HashEntry<K, V> *entry;
    HashEntry<K, V> *next;

    for(uint64_t i = 0; i < m_size; i++)
    {
        entry = m_table[i];
        
        while(entry != NULL)
        {
            next = entry->next();
            delete entry;
            entry = next;
        }
    }

    delete[] m_table;
    m_size = 128;
    m_table = new HashEntry<K, V>*[m_size];

    for(uint64_t i = 0; i < m_size; i++)
    {
        m_table[i] = NULL;
    }
}

//------------------------------------
// HashEntry Definition
//------------------------------------

template <class K, class V>
class HashEntry
{
    public:
        HashEntry();
        HashEntry(K key, V value);
        ~HashEntry();

        void set(K key, V value);

        K key();
        V value();

        void setPrevious(HashEntry<K, V> *prev);
        void setNext(HashEntry<K, V> *next);

        HashEntry<K, V> *previous();
        HashEntry<K, V> *next();

        bool isNull();

    private:
        K m_key;
        V m_value;
        HashEntry<K, V> *m_prev;
        HashEntry<K, V> *m_next;
        bool m_null;
};

//----------------------------------
// HashEntry Members
//----------------------------------

template<class K, class V>
HashEntry<K, V>::HashEntry()
{
    m_null = true;
    m_next = NULL;
    m_prev = NULL;
}

template<class K, class V>
HashEntry<K, V>::HashEntry(K key, V value)
{
    m_key = key;
    m_value = value;
    m_null = false;
    m_next = NULL;
    m_prev = NULL;
}

template<class K, class V>
HashEntry<K, V>::~HashEntry()
{
    m_next = NULL;
    m_prev = NULL;
}

template<class K, class V>
void HashEntry<K, V>::set(K key, V value)
{
    m_key = key;
    m_value = value;
    m_null = false;
}

template<class K, class V>
K HashEntry<K, V>::key()
{
    return m_key;
}

template<class K, class V>
V HashEntry<K, V>::value()
{
    return m_value;
}

template<class K, class V>
void HashEntry<K, V>::setPrevious(HashEntry<K, V> *prev)
{
    m_prev = prev;
}

template<class K, class V>
void HashEntry<K, V>::setNext(HashEntry<K, V> *next)
{
    m_next = next;
}

template<class K, class V>
HashEntry<K, V> *HashEntry<K, V>::previous()
{
    return m_prev;
}

template<class K, class V>
HashEntry<K, V> *HashEntry<K, V>::next()
{
    return m_next;
}

template<class K, class V>
bool HashEntry<K, V>::isNull()
{
    return m_null;
}

#endif
        
