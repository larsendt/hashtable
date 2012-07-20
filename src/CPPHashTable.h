#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "dbg.h"
#include <openssl/md5.h>
#include <cstdlib>

#include <vector>

template <class K, class V>
class HashEntry;

#define INITIAL_SIZE 10000

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
        std::vector<K> keys();

        void resizeTo(unsigned int new_size);
        unsigned int hashIndex(K key);
        unsigned int collisions();

    private:
        void init(unsigned int size);

        HashEntry<K, V> **m_table;
        unsigned int m_size;    
        unsigned int m_coll;    
};

//-------------------------------------
// HashTable Members
//-------------------------------------

template <class K, class V>
HashTable<K, V>::HashTable()
{
    init(INITIAL_SIZE);
}

template <class K, class V>
HashTable<K, V>::~HashTable()
{
    HashEntry<K, V> *entry;
    HashEntry<K, V> *next;
    for(unsigned int i = 0; i < m_size; i++)
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
    unsigned int index = hashIndex(key);
    HashEntry<K, V> *entry = new HashEntry<K, V>(key, value);
    entry->setNext(NULL);
    entry->setPrevious(NULL);

    HashEntry<K, V> *parent = m_table[index];
    
    if(parent == NULL)
    {
        m_table[index] = entry;
        return;
    }
    else
    {
        m_coll++;
        //TODO: this borks
        //resizeTo(m_size + INITIAL_SIZE);
    }


    while(parent->next() != NULL)
    {
        parent = parent->next();
    }

    parent->setNext(entry);
    entry->setPrevious(parent);
}

template <class K, class V>
V HashTable<K, V>::value(K key)
{
    unsigned int index = hashIndex(key);
    HashEntry<K, V> *entry = m_table[index];

    if(entry == NULL)
    {
        debug("HashTable: No corresponding value for index %d", index);
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
        debug("HashTable: Found entry for index %d but no matching key", index);
        return V();
    }
}

template <class K, class V>
bool HashTable<K, V>::contains(K key)
{
    unsigned int index = hashIndex(key);
    HashEntry<K, V> *entry = m_table[index];

    if(entry == NULL)
        return false;

    while(entry->key() != key && entry->next() != NULL)
    {
        entry = entry->next();
    }

    return (entry->key() == key);
}

template<class K, class V>
std::vector<K> HashTable<K, V>::keys()
{
    std::vector<K> key_vector;
    HashEntry<K, V> *entry;

    for(unsigned int i = 0; i < m_size; i++)
    {
        entry = m_table[i];
        
        while(entry != NULL)
        {
            key_vector.push_back(entry->key());
            entry = entry->next();
        }
    }

    return key_vector;
}

template <class K, class V>
unsigned int HashTable<K, V>::hashIndex(K key)
{
    //TODO is this stupid?
    unsigned char* md5_hash = MD5((const unsigned char*)(&key), sizeof(K), NULL);

    //TODO is this also really stupid?
    unsigned int index;
    memcpy(&index, md5_hash, sizeof(index));
    index %= m_size;
    return index;
}

template <class K, class V>
unsigned int HashTable<K, V>::collisions()
{
    return m_coll;
}

template <class K, class V>
void HashTable<K, V>::resizeTo(unsigned int new_size)
{
    debug("YOU SHOULD NOT BE HERE. GO AWAY.");
    
    unsigned int old_size = m_size;
    m_size = new_size;

    std::vector< HashEntry<K, V>* > entry_vector;
    HashEntry<K, V> *entry;

    for(unsigned int i = 0; i < old_size; i++)
    {
        entry = m_table[i];

        while(entry != NULL)
        {
            entry_vector.push_back(entry);
            entry = entry->next();
        }
    }

    delete[] m_table;
    m_table = new HashEntry<K, V>*[new_size];

    HashEntry<K, V> *parent;

    for(unsigned int i = 0; i < entry_vector.size(); i++)
    {
        entry = entry_vector[i];
        unsigned int index = hashIndex(entry->key());
        parent = m_table[index];

        if(parent == NULL)
        {
            m_table[index] = entry;
            continue;
        }
        else
        {
            m_coll++;
            while(parent->next() != NULL)
            {
                parent = parent->next();
            }

            parent->setNext(entry);
        }
    }
}

template <class K, class V>
void HashTable<K, V>::clear()
{
    m_coll = 0;
    HashEntry<K, V> *entry;
    HashEntry<K, V> *next;

    for(unsigned int i = 0; i < m_size; i++)
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
    init(INITIAL_SIZE);
}

template <class K, class V>
void HashTable<K, V>::init(unsigned int size)
{
    m_size = size;
    m_coll = 0;
    m_table = new HashEntry<K, V>*[m_size];

    for(unsigned int i = 0; i < m_size; i++)
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
        
