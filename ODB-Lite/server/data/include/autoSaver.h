//
// Created by fiy-pc on 2024/10/4.
//

#ifndef AUTOSAVER_H
#define AUTOSAVER_H
#include "SDS.h"
#include "diskio.h"

// 用外部变量记录变化数
extern int change_count;
// 用外部变量控制线程关闭
extern int autoSaverSwitch;
// 操作变化数的函数
void increment_change_count();
void reset_change_count();
int get_change_count();

// 变化监视器线程相关函数与定义
typedef struct
{
    HashTable *ht;
    SDS time;
    SDS changeNum;
    char fileName[128];
}autoSaverArgs;

SDS autoSaver_create(HashTable *ht, const char *filename,SDS time,SDS changeNum);
void *autoSaver(void *arg);
#endif //AUTOSAVER_H
