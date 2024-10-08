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
#define MAX_PARAM_NUM 4
#define ORP_VERSION_LEN 5

// 客户端规定
#define MAX_AFFAIR_SIZE 1000

// 包格式
#define MAX_LINES_NUM_BYTES 4
#define MAX_LINES_NUM 9999
#define MAX_LINE_LEN 512
#define LINE_END_SIGN "\n"
#define ORP_OK_SIGN "orpok"
#define ORP_ERROR_SIGN "orperr"

const char* ORPGetVersion();


#endif //ORPSET_H
