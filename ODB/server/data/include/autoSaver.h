//
// Created by fiy-pc on 2024/10/4.
//

#ifndef AUTOSAVER_H
#define AUTOSAVER_H
#include "SDS.h"
#include "Realizer.h"

// 用外部变量记录变化数
extern int change_count;
// 用外部变量控制线程关闭
extern int autoSaverSwitch;
extern double saveTime;
// 操作变化数的函数
void increment_change_count();
void reset_change_count();
int get_change_count();

// 操作计时的函数
void increment_Time();
void reset_Time();
double get_Time();

// 操作变化监视器线程开关的函数
void open_autoSaver();
void close_autoSaver();

// 参数包
typedef struct
{
    HashTable *ht;
    SDS time;
    SDS changeNum;
    char fileName[256];
}autoSaverArgs;

// 变化监视器线程相关函数
SDS autoSaver_create(HashTable *ht, const char *filename,SDS time,SDS changeNum);
void *autoSaver(void *arg);
#endif //AUTOSAVER_H
