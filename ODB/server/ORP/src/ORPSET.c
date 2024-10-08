//
// Created by fiy-pc on 2024/10/2.
//

#include "ORPSET.h"

#include <stdio.h>

#define ORP_MAJOR_VERSION 1
#define ORP_MINOR_VERSION 0

const char* ORPGetVersion() {
    // 使用静态字符数组来存储版本号字符串
    static char version[ORP_VERSION_LEN];
    // 使用 sprintf 函数格式化字符串
    sprintf(version, "%d.%d", ORP_MAJOR_VERSION, ORP_MINOR_VERSION);
    // 返回版本字符串
    return version;
}