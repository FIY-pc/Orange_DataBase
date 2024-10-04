//
// Created by fiy-pc on 2024/10/4.
//

#include "autoSaver.h"

#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <bits/pthreadtypes.h>
// 定义全局变量
int change_count = 0;
int autoSaverSwitch = 0;
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
SDS autoSaver_create(HashTable *ht, const char *filename,SDS time,SDS changeNum)
{
    if (strcmp(time.data,"0") == 0 || strcmp(changeNum.data,"0") == 0)
    {
        return sds_new("autoSaver not be created because auto saving is closed");
    }
    // 打开开关
    autoSaverSwitch = 1;
    // 传参结构体
    autoSaverArgs args;
    args.ht = ht;
    args.time = time;
    args.changeNum = changeNum;
    strcpy(args.fileName,filename);

    pthread_t autoSaver_thread;
    pthread_create(&autoSaver_thread,NULL,autoSaver,(void*)&args);
    return sds_new("autoSaver created");
}

// 变化监控器线程函数
void *autoSaver(void *arg)
{
    autoSaverArgs *args = (autoSaverArgs *)arg;
    SDS SDStime = args->time;
    SDS SDSchangeNum = args->changeNum;
    int time,changeNum;
    sscanf(SDStime.data,"%ld",&time);
    sscanf(SDSchangeNum.data,"%ld",&changeNum);
    while (1)
    {
        if(autoSaverSwitch==0)
        {
            pthread_exit(NULL);
        }
        if(change_count>=changeNum)
        {
            odbrgsave(args->ht,args->fileName);
        }
        sleep(time);
    }
}