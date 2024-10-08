//
// Created by fiy-pc on 2024/10/5.
//

#include "valueHash.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int value_hash_function(const char *key) {
    unsigned int hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % VALUE_HASH_INIT_SIZE;
}

const char *valueHashGet(valueHash *vht, const char *key) {
    unsigned int index = value_hash_function(key);
    valueEntry *entry = vht->valueEntries[index];
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

void valueHashSet(valueHash *vht, const char *key, const char *value) {
    if (value == NULL )
    {
        perror("valueHashSet: vht is null");
        return;
    }
    unsigned int index = value_hash_function(key);

    valueEntry *newEntry = (valueEntry *)malloc(sizeof(valueEntry));

    if (!newEntry) {

        printf("Memory allocation failed for valueHashSet\n");

        return;

    }

    strncpy(newEntry->key, key, MAX_KEY_LEN);

    newEntry->key[MAX_KEY_LEN - 1] = '\0';

    strncpy(newEntry->value, value, MAX_VALUE_LEN);

    newEntry->value[MAX_VALUE_LEN - 1] = '\0';

    newEntry->next = vht->valueEntries[index]; // 发生段错误

    vht->valueEntries[index] = newEntry;

}

void valueHashDelete(valueHash *vht, const char *key) {
    unsigned int index = value_hash_function(key);
    valueEntry *entry = vht->valueEntries[index];
    valueEntry *prev = NULL;
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            if (prev) {
                prev->next = entry->next;
            } else {
                vht->valueEntries[index] = entry->next;
            }
            free(entry);
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}

valueHash *SDS_to_valueHash(SDS formatHash) {
    if (!formatHash.data || formatHash.len < 2 || formatHash.data[0] != '{' || formatHash.data[formatHash.len - 1] != '}') {
        return NULL;
    }
    valueHash *vht = (valueHash *)malloc(sizeof(valueHash));
    if (!vht) {
        printf("Memory allocation failed for valueHash\n");
        return NULL;
    }
    memset(vht->valueEntries, 0, sizeof(vht->valueEntries));
    char *start = formatHash.data + 1;
    char *end = formatHash.data + formatHash.len - 1;
    char *current = start;
    while (current < end) {
        // 找键
        char *key_start = current;
        while (current < end && !isspace(*current) && *current != ':') {
            current++;
        }
        if (current >= end) break;
        *current = '\0';

        char *key = strdup(key_start);
        *current = ':';
        current++;
        while (current < end && isspace(*current)) {
            current++;
        }
        // 找值
        char *value_start = current;
        while (current < end && *current != ';') {
            current++;
        }
        if (*current == ';' || *current == '}') {
            *current = '\0';
        }
        char *value = strdup(value_start);
        if (*current == ';') {
            *current = ';';
        }

        valueHashSet(vht, key, value);
        current++;
        while (current < end && isspace(*current)) {
            current++;
        }
    }
    return vht;
}

SDS valueHash_to_SDS(valueHash *vht) {
    if (!vht) {
        return sds_new("");
    }
    SDS result = sds_new("{");
    char buffer[MAX_KEY_LEN + MAX_VALUE_LEN + 3];
    int first = 1;
    for (int i = 0; i < VALUE_HASH_INIT_SIZE; i++) {
        valueEntry *entry = vht->valueEntries[i];
        while (entry) {
            if (!first) {
                sds_set(&result, strcat(result.data, ";"));
            } else {
                first = 0;
            }
            snprintf(buffer, sizeof(buffer), "%s:%s", entry->key, entry->value);
            SDS new_part = sds_new(buffer);
            char *temp = realloc(result.data, result.len + new_part.len + 1);
            if (!temp) {
                fprintf(stderr, "Memory allocation failed while building SDS\n");
                sds_free(&new_part);
                sds_free(&result);
                return (SDS){0, NULL};
            }
            result.data = temp;
            strcpy(result.data + result.len, new_part.data);
            result.len += new_part.len;
            sds_free(&new_part);
            entry = entry->next;
        }
    }
    SDS final_result = sds_new(result.data);
    sds_set(&final_result, strcat(final_result.data, "}"));
    sds_free(&result);
    return final_result;
}

