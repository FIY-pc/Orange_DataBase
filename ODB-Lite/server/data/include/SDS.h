//
// Created by fiy-pc on 2024/10/1.
//

#ifndef SDS_H
#define SDS_H
#include <stddef.h>

typedef struct
{
    int len;
    char *data;
}SDS;

SDS sds_new(const char *str);
SDS sds_cpy(const SDS sds);
void sds_free(SDS *sds);
void sds_set(SDS *sds, const char *data) ;
void sds_print(SDS *sds);
void printSDSArray(SDS *arr, int size);
void freeSDSArray(SDS *arr, int size);
int sds_cmp(const SDS sds1, const SDS sds2);
int sds_len(SDS *sds);
char *sds_get(const SDS *sds);
#endif //SDS_H
