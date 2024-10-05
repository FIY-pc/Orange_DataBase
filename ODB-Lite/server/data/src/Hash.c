//
// Created by fiy-pc on 2024/10/3.
//

#include "Hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEY_LEN 256
#define MAX_VALUE_LEN 256
#define HASH_TABLE_INIT_SIZE 100

void hashInit(HashTable *ht) {
    for (int i = 0; i < HASH_TABLE_INIT_SIZE; i++) {
        ht->entries[i] = NULL;
    }
}

unsigned int hash_function(const char *key) {
    unsigned int hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % HASH_TABLE_INIT_SIZE;
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
    new_entry->is_modified = 1;
    new_entry->is_in_hash_table = 1; // 设置新标志

    // 如果键已存在，则需要更新现有条目的值和修改标志
    Entry *existing = ht->entries[index];
    while (existing) {
        if (strcmp(existing->key, key) == 0) {
            strncpy(existing->value, value, MAX_VALUE_LEN);
            existing->is_modified = 1;
            existing->is_in_hash_table = 1; // 确保此标志被设置
            free(new_entry);
            return;
        }
        existing = existing->next;
    }

    // 如果键不存在，则将新条目添加到链表的头部
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

void hashDelete(HashTable *ht, const char *key) {
    unsigned int index = hash_function(key);
    Entry *entry = ht->entries[index];
    Entry *prev = NULL;
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            if (prev) {
                prev->next = entry->next;
            } else {
                ht->entries[index] = entry->next;
            }
            free(entry);
            printf("Deleted entry with key: %s\n", key);
            return;
        }
        prev = entry;
        entry = entry->next;
    }
    printf("Key %s not found in hash table.\n", key);
}

void freeHashTable(HashTable *ht) {
    for (int i = 0; i < HASH_TABLE_INIT_SIZE; i++) {
        Entry *entry = ht->entries[i];
        while (entry) {
            Entry *temp = entry;
            entry = entry->next;
            free(temp);
        }
        ht->entries[i] = NULL;
    }
}

void hash_load_from_file(HashTable *ht, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        if (!isFileEmpty(filename)) {
            fprintf(stderr, "Failed to open file for reading\n");
            exit(EXIT_FAILURE);
        }
        return;
    }

    char line[MAX_KEY_LEN + MAX_VALUE_LEN + 2];
    while (fgets(line, sizeof(line), file)) {
        char key[MAX_KEY_LEN];
        char value[MAX_VALUE_LEN];
        if (sscanf(line, "%[^=]=%[^\n]", key, value) == 2) {
            hashSet(ht, key, value);
            // 查找刚加载的条目并设置 is_loaded 和 is_in_hash_table 标志
            unsigned int index = hash_function(key);
            Entry *entry = ht->entries[index];
            while (entry) {
                if (strcmp(entry->key, key) == 0) {
                    entry->is_loaded = 1;
                    entry->is_in_hash_table = 1;
                    break;
                }
                entry = entry->next;
            }
        }
    }
    fclose(file);
}

void hash_save_to_file(HashTable *ht, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Failed to open file for writing\n");
        return;
    }

    for (int i = 0; i < HASH_TABLE_INIT_SIZE; i++) {
        Entry *entry = ht->entries[i];
        while (entry) {
            if (entry->is_in_hash_table) { // 只检查是否在哈希表中
                fprintf(file, "%s=%s\n", entry->key, entry->value);
                // 可以选择性地重置 is_modified 标志
                // entry->is_modified = 0;
            }
            entry = entry->next;
        }
    }
    fclose(file);
}

int isFileEmpty(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    return size == 0;
}