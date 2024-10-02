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
void sds_free(SDS *sds);
char *sds_get(SDS *sds);
void sds_set(SDS *sds, char *data);
int sds_len(SDS *sds);
void sds_print(SDS *sds);
SDS sds_cpy(const SDS s);
int sds_cmp(const SDS s1, const SDS s2);
#endif //SDS_H
