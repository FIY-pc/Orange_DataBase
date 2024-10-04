//
// Created by fiy-pc on 2024/10/4.
//

#ifndef AUTOSAVER_H
#define AUTOSAVER_H
#include "SDS.h"
#include "diskio.h"

// 用外部变量记录变化数
extern int change_count;
// 操作变化数的函数
void increment_change_count();
void reset_change_count();
int get_change_count();

void AutoSaver_create(HashTable ht,SDS time,SDS changeNum);

#endif //AUTOSAVER_H
