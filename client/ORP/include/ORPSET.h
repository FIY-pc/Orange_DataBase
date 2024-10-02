//
// Created by fiy-pc on 2024/10/1.
//

#ifndef ORPSET_H
#define ORPSET_H
#include "SDS.h"

// 协议版本
#define ORP_VERSION "1.0"

typedef struct
{
    char type;
    SDS sds;
}SDSData;

typedef struct
{
    char version[4];
    char method;
    void *ORPData;
}RQStream;

typedef struct
{
    char version[4];
    char status[4];
    void *ORPData;
}RPStream;
#define SDSTYPE '1'

#endif //ORPSET_H
