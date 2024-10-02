#include "SDS.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SDS sds_new(const char *str) {
    SDS s;
    s.len = 0;
    s.data = NULL;
    if (str) {
        while (*str) s.len++;
        s.data = (char *)malloc(s.len + 1);
        if (s.data) {
            for (int i = 0; i < s.len; i++) {
                s.data[i] = str[i];
            }
            s.data[s.len] = '\0';
        }
    }
    return s;
}

void sds_free(SDS *sds)
{
    if (sds == NULL)
        return;
    free(sds->data);
    free(sds);
}

char *sds_get(SDS *sds)
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

SDS sds_cpy(const SDS s) {
    const SDS new_s = sds_new(s.data);
    return new_s;
}