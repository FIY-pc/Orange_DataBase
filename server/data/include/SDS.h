//
// Created by fiy-pc on 2024/10/1.
//

#ifndef SDS_H
#define SDS_H
#include <stddef.h>

typedef struct
{
    size_t len;
    char *data;
}SDS;

SDS *SDS_New(char *data);
void SDS_Free(SDS *sds);
char *SDS_Get(SDS *sds);
void SDS_Set(SDS *sds, char *data);
#endif //SDS_H
