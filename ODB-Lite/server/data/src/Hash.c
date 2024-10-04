//
// Created by fiy-pc on 2024/10/3.
//

#include "Hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void hashInit(HashTable *ht)
{
    printf("init ht\n");
    for (int i=0;i<HASH_TABLE_INIT_SIZE;i++)
    {
        ht->entries[i] = NULL;
    }
    printf("ht init done\n");
}

unsigned int hash_function(const char *key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash << 5) + *key++;
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
    Entry *prev = NULL; // 用于跟踪前一个条目，以便在需要时修改其 next 指针

    // 遍历链表，查找要删除的条目
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            // 找到了要删除的条目
            if (prev) {
                prev->next = entry->next; // 修改前一个条目的 next 指针
            } else {
                ht->entries[index] = entry->next; // 如果没有前一个条目，则直接修改桶的指针
            }
            free(entry); // 释放条目的内存
            entry = NULL;
            printf("Deleted entry with key: %s\n", key);
            return; // 完成删除后返回
        }
        prev = entry; // 在移动到下一个条目之前，更新 prev 指针
        entry = entry->next; // 移动到下一个条目
    }
    // 如果没有找到匹配的条目，则什么也不做，但可以选择打印一条消息
    printf("Key %s not found in hash table.\n", key);
}

void freeHashTable(HashTable *ht) {
    // 遍历哈希表的每一个桶
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

void hash_save_to_file(HashTable *ht, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Failed to open file for writing\n");
        return;
    }
    for (int i = 0; i < HASH_TABLE_INIT_SIZE; i++) {
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
    if (isFileEmpty(filename)) {
        // 文件为空，不需要加载任何数据
        return;
    }
    if (!file) {
        fprintf(stderr, "Failed to open file for reading\n");
        exit(EXIT_FAILURE);
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
