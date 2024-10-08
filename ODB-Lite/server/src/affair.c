//
// Created by fiy-pc on 2024/10/4.
//

#include "affair.h"

#include <ORPSET.h>
#include <pthread.h>

#include "command.h"

#include <stdio.h>
#include <string.h>
#include <transmit.h>
#include <unistd.h>
pthread_mutex_t ht_lock = PTHREAD_MUTEX_INITIALIZER;

void affair_handler(HashTable **ht,int clientfd,SDS *affair,int isautocommit)
{
    // 创建快照
    pthread_mutex_lock(&ht_lock);
    HashTable *snapshot = copyHashTable(*ht);
    pthread_mutex_unlock(&ht_lock);

    SDS *current=affair;
    if(isautocommit==1)
    {
        commands_handler(snapshot,clientfd,current);
        pthread_mutex_lock(&ht_lock);
        *ht = copyHashTable(snapshot);
        pthread_mutex_unlock(&ht_lock);
        freeHashTable(snapshot);
        return;
    }
    if(isautocommit==0 && strncmp(affair[0].data,"begin",5)!=0)
    {
        char *errmessage[1];
        errmessage[0] = strdup("illegal Input");
        writelines(clientfd,errmessage,1);
        freeHashTable(snapshot);
        return;
    }

    // 计算总命令数
    int totalCount = 0;
    for (totalCount = 0; totalCount<MAX_AFFAIR_SIZE;totalCount++)
    {
        if(affair[totalCount].data==NULL)break;
    }

    // 执行命令
    int successCount = commands_handler(snapshot,clientfd,current+1);
    if(successCount != totalCount-1)
    {
        freeHashTable(snapshot);
        return;
    }
    // 用修改后的快照覆盖原哈希表
    pthread_mutex_lock(&ht_lock);
    replaceHashTable(ht,snapshot);
    pthread_mutex_unlock(&ht_lock);
    snapshot = NULL;
}
