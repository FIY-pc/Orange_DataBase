//
// Created by fiy-pc on 2024/10/1.
//

#ifndef ORPSET_H
#define ORPSET_H
#include "SDS.h"

// 协议版本
#define ORP_VERSION "1.0"
#define MAX_PARAM_LEN 256
#define MAX_PARAM_NUM 2
// 类型标识符
#define SDSTYPE '1'

// 命令种类
typedef enum {
    COMMAND_SET,
    COMMAND_GET,
    COMMAND_DELETE,
    COMMAND_UNKNOWN
} CommandType;

// 数据包格式
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


#endif //ORPSET_H
