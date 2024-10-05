//
// Created by fiy-pc on 2024/10/5.
//

#ifndef VALUEHASH_H
#define VALUEHASH_H
#include <DataSet.h>
#include "Hash.h"
#include "SDS.h"


typedef struct valueEntry
{
    char key[MAX_KEY_LEN];
    char value[MAX_VALUE_LEN];
    struct valueEntry *next;
}valueEntry;

typedef struct valueHash
{
    valueEntry *valueEntries[HASH_TABLE_INIT_SIZE];
}valueHash;

const char *valueHashGet(valueHash *vht, const char *key);
void valueHashSet(valueHash *vht,const char *key,const char *value);
void valueHashDelete(valueHash *vht, const char *key);
valueHash *SDS_to_valueHash(SDS formatHash);
SDS valueHash_to_SDS(valueHash *vht);
#endif //VALUEHASH_H