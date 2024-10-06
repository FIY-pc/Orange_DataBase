#include "SDS.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SDS sds_new(const char *str) {
    SDS s = {0, NULL};
    if (str == NULL) {
        fprintf(stderr, "Input string is NULL\n");
        return s;
    }
    s.len = strlen(str);
    s.data = (char *)calloc(s.len + 1, sizeof(char));
    if (s.data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return s;
    }
    strcpy(s.data, str);
    return s;
}

void sds_free(SDS *sds)
{
    if (sds == NULL)
        return;
    if (sds->data)
    {
        free(sds->data);
        sds->data = NULL;
    }
}

char *sds_get(const SDS *sds)
{
    if (sds == NULL)
        return NULL;
    return sds->data;
}

void sds_set(SDS *sds, const char *data) {
    if (sds == NULL || data == NULL) return;
    int newDataLen = strlen(data);
    char *newData = (char *)realloc(sds->data, newDataLen + 1);
    if (newData == NULL) {
        fprintf(stderr, "Reallocation failed\n");
        return;
    }
    sds->data = newData;
    strcpy(sds->data, data);
    sds->len = newDataLen;
}

int sds_len(SDS *sds)
{
    if (sds == NULL)
        return 0;
    return sds->len;
}

SDS sds_cpy(const SDS sds) {
    const SDS new_sds = sds_new(sds.data);
    return new_sds;
}

void sds_print(SDS *sds)
{
    if (sds == NULL)
        return;
    printf("%s\n", sds->data);
}

int sds_cmp(const SDS sds1, const SDS sds2)
{
    if(sds1.len != sds2.len)
    {
        return sds1.len>sds2.len ? 1 : -1;
    }
    int minlen = (sds1.len < sds2.len) ? sds1.len : sds2.len;
    for (int i = 0; i < minlen; i++)
    {
        if(sds1.data[i] != sds2.data[i])
            return sds1.data[i]>sds2.data[i] ? 1 : -1;
    }
    return 0;
}

char* strcat_sds(SDS *sds, SDS *add) {
    size_t newlen = sds->len + add->len;
    char *newdata = realloc(sds->data, newlen + 1);
    if (!newdata) {
        fprintf(stderr, "Memory allocation failed while concatenating SDS\n");
        return NULL;
    }
    sds->data = newdata;
    memcpy(sds->data + sds->len, add->data, add->len);
    sds->data[newlen] = '\0';
    sds->len = newlen;
    return sds->data;
}

void freeSDSArray(SDS *arr, int size) {
    for (int i = 0; i < size; i++) {
        sds_free(&arr[i]);
    }
}

void printSDSArray(SDS *arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("Param %d: %s\n", i, arr[i].data);
    }
}

