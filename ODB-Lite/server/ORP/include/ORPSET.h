//
// Created by fiy-pc on 2024/10/1.
//

#ifndef ORPSET_H
#define ORPSET_H
#include "SDS.h"
#include <stdlib.h>

// 协议版本
#define ORP_MAJOR_VERSION 1
#define ORP_MINOR_VERSION 0

// 命令参数规定
#define MAX_PARAM_LEN 256
#define MAX_PARAM_NUM 2
#define ORP_VERSION_LEN 5

// 客户端规定
#define MAX_AFFAIR_SIZE 1000

// 命令种类
typedef enum {
    COMMAND_SET,
    COMMAND_GET,
    COMMAND_DELETE,
    COMMAND_SAVE,
    COMMAND_RGSAVE,
    COMMAND_UNKNOWN
} CommandType;

// 数据包格式
typedef struct
{
    char type;
    int32_t len;
    SDS sds;
}SDSData;

const char* ORPGetVersion();


#endif //ORPSET_H
