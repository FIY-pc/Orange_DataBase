#include "SDS.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SDS *SDS_New(char *data)
{
    //分配内存
    SDS *sds = (SDS *)malloc(sizeof(SDS));
    if (sds == NULL)
    {
        printf("SDS_New: malloc failed\n");
        return NULL;
    }
    //复制字符串与计算长度
    int dataLen = 0;
    char *temp = data;
    while (*temp != '\0')
    {
        dataLen++;
        temp++;
    }
    temp = data;
    sds->data = (char *)malloc((dataLen+1) * sizeof(char));
    if (sds->data == NULL)
    {
        printf("SDS_New: malloc failed\n");
        free(sds);
        return NULL;
    }
    for (int i = 0; i < dataLen; i++)
    {
        sds->data[i] = *temp;
        temp++;
    }
    sds->data[dataLen] = '\0';
    return sds;
}

void SDS_Free(SDS *sds)
{
    if (sds == NULL)
        return;
    free(sds->data);
    free(sds);
}

char *SDS_Get(SDS *sds)
{
    if (sds == NULL)
        return NULL;
    return sds->data;
}

void SDS_Set(SDS *sds, char *data){
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
        printf("SDS_Set: realloc failed\n");
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