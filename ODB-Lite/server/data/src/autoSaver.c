//
// Created by fiy-pc on 2024/10/4.
//

#include "autoSaver.h"

#include <unistd.h>
// 定义全局变量
int change_count = 0;

// 增加变化数
void increment_change_count() {
    change_count++;
}
// 重置变化数
void reset_change_count() {
    change_count = 0;
}
// 获取当前变化数
int get_change_count() {
    return change_count;
}
// 变化监控器线程创建
void AutoSaver_create(HashTable ht,SDS time,SDS changeNum)
{

}