#include "SDS.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SDS sds_new(const char *str) {
    SDS s;
    s.len = 0;
    s.data = NULL;
    // 检查输入指针是否为空
    if (str == NULL) {
        fprintf(stderr, "Input string is NULL\n");
        return s; // 返回一个空的SDS结构
    }
    s.len = strlen(str);
    s.data = (char *)malloc(s.len + 1);
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
    free(sds->data);
    sds->data = NULL;
}

char *sds_get(const SDS *sds)
{
    if (sds == NULL)
        return NULL;
    return sds->data;
}

void sds_set(SDS *sds, char *data){
    if (sds == NULL)
        return;
    // 计算新字符串的长度
    int newDataLen = 0;
    char *temp = data;
    while (*temp != '\0')
    {
        newDataLen++;
        temp++;
    }
    // 重新分配内存
    char *newData = (char *)realloc(sds->data, (newDataLen + 1) * sizeof(char));
    if (newData == NULL)
    {
        printf("sds_set: realloc failed\n");
        return;
    }
    // 复制字符串
    sds->data = newData;
    temp = data;
    for (int i = 0; i < newDataLen; i++)
    {
        sds->data[i] = *temp;
        temp++;
    }
    sds->len = newDataLen;
    sds->data[newDataLen] = '\0';
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

int sds_cmp(const SDS s1, const SDS s2)
{
    if(s1.len != s2.len)
    {
        return s1.len>s2.len ? 1 : -1;
    }
    int minlen = (s1.len < s2.len) ? s1.len : s2.len;
    for (int i = 0; i < minlen; i++)
    {
        if(s1.data[i] != s2.data[i])
            return s1.data[i]>s2.data[i] ? 1 : -1;
    }
    return 0;
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