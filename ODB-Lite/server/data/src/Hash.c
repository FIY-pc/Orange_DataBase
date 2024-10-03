//
// Created by fiy-pc on 2024/10/3.
//

#include "Hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int hash_function(const char *key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash << 5) + *key++;
    }
    return hash % HASH_TABLE_SIZE;
}

void hashSet(HashTable *ht, const char *key, const char *value) {
    unsigned int index = hash_function(key);
    Entry *new_entry = (Entry *)malloc(sizeof(Entry));
    if (!new_entry) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    strncpy(new_entry->key, key, MAX_KEY_LEN);
    strncpy(new_entry->value, value, MAX_VALUE_LEN);
    new_entry->next = ht->entries[index];
    ht->entries[index] = new_entry;
}

const char *hashGet(HashTable *ht, const char *key) {
    unsigned int index = hash_function(key);
    Entry *entry = ht->entries[index];
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

void freeHashTable(HashTable *ht) {
    // 遍历哈希表的每一个桶
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Entry *entry = ht->entries[i];
        while (entry) {
            Entry *temp = entry;
            entry = entry->next;
            free(temp);
        }
        ht->entries[i] = NULL;
    }
}

void hash_save_to_file(HashTable *ht, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Failed to open file for writing\n");
        return;
    }
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Entry *entry = ht->entries[i];
        while (entry) {
            fprintf(file, "%s=%s\n", entry->key, entry->value);
            entry = entry->next;
        }
    }
    fclose(file);
}

void hash_load_from_file(HashTable *ht, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Failed to open file for reading\n");
        return;
    }
    char line[MAX_KEY_LEN + MAX_VALUE_LEN + 2];
    while (fgets(line, sizeof(line), file)) {
        char key[MAX_KEY_LEN];
        char value[MAX_VALUE_LEN];
        if (sscanf(line, "%[^=]=%[^\n]", key, value) == 2) {
            hashSet(ht, key, value);
        }
    }
    fclose(file);
}
