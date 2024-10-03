//
// Created by fiy-pc on 2024/10/3.
//

#ifndef HASH_H
#define HASH_H
#include "DataSet.h"

typedef struct Entry {
    char key[MAX_KEY_LEN];
    char value[MAX_VALUE_LEN];
    struct Entry *next;
} Entry;

typedef struct HashTable {
    Entry *entries[HASH_TABLE_SIZE];
} HashTable;

unsigned int hash_function(const char *key);
void hashSet(HashTable *ht, const char *key, const char *value);
const char *hashGet(HashTable *ht, const char *key);
void freeHashTable(HashTable *ht);
void hash_save_to_file(HashTable *ht, const char *filename);
void hash_load_from_file(HashTable *ht, const char *filename);
#endif //HASH_H
