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
SDS sds_cpy(const SDS s);
void sds_free(SDS *sds);
void sds_set(SDS *sds, char *data);
void sds_print(SDS *sds);
int sds_cmp(const SDS s1, const SDS s2);
int sds_len(SDS *sds);
char *sds_get(SDS *sds);
#endif //SDS_H
