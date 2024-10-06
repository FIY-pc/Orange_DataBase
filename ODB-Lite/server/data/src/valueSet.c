//
// Created by fiy-pc on 2024/10/5.
//

#include "valueSet.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

valueHash *SDS_to_valueSet(SDS formatSet) {
    if (!formatSet.data || formatSet.len < 2 || formatSet.data[0] != '(' || formatSet.data[formatSet.len - 1] != ')') {
        printf("Invalid format set\n");
        return NULL;
    }
    valueHash *vht = (valueHash *)malloc(sizeof(valueHash));
    if (!vht) {
        printf("Memory allocation failed for valueHash\n");
        return NULL;
    }
    memset(vht->valueEntries, 0, sizeof(vht->valueEntries));
    char *start = formatSet.data + 1;
    char *end = formatSet.data + formatSet.len - 1;
    char *current = start;
    while (current < end) {
        char *value_start = current;
        while (current < end && !isspace(*current) && *current != ';' && *current != ')') {
            current++;
        }
        if (current >= end) break;
        *current = '\0';
        char *value = strdup(value_start);
        *current = ';';
        valueHashSet(vht, value, value);
        current++;
        while (current < end && isspace(*current)) {
            current++;
        }
    }
    return vht;
}

SDS valueSet_to_SDS(valueHash *valueSet) {
    if (!valueSet) {
        return sds_new("");
    }
    SDS result = sds_new("(");
    char buffer[MAX_KEY_LEN + 3];
    int first = 1;
    for (int i = 0; i < VALUE_HASH_INIT_SIZE; i++) {
        valueEntry *entry = valueSet->valueEntries[i];
        while (entry) {
            if (!first) {
                SDS temp = sds_new(";");
                sds_set(&result, strcat_sds(&result, &temp));
                sds_free(&temp);
            } else {
                first = 0;
            }
            snprintf(buffer, sizeof(buffer), "%s", entry->key);
            SDS new_part = sds_new(buffer);
            sds_set(&result, strcat_sds(&result, &new_part));
            sds_free(&new_part);
            entry = entry->next;
        }
    }
    SDS final_result = sds_new(result.data);
    SDS end = sds_new(")");
    sds_set(&final_result, strcat_sds(&final_result, &end));
    sds_free(&result);
    return final_result;
}