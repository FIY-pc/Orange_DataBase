//
// Created by fiy-pc on 2024/10/4.
//

#include "affair.h"

#include <ORPSET.h>

#include "command.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

void affair_handler(HashTable **ht,int clientfd,SDS *affair,int isautocommit)
{
    // 创建快照
    HashTable *snapshot = copyHashTable(*ht);

    SDS *current=affair;
    if(isautocommit)
    {
        commands_handler(snapshot,clientfd,current);
        return;
    }

    if(strncmp(affair->data,"begin",5)!=0)
    {
        write(clientfd,"1:Illegal Input\n",strlen("1:Illegal Input\n"));
        return;
    }

    // 计算总命令数
    int totalCount = 0;
    for (totalCount = 0; totalCount<MAX_AFFAIR_SIZE;totalCount++)
    {
        if(affair[totalCount].data==NULL)break;
    }

    // 执行命令
    int successCount = commands_handler(snapshot,clientfd,current);
    if(successCount != totalCount)
    {
        freeHashTable(snapshot);
        return;
    }
    // 用修改后的快照覆盖原哈希表
    *ht = copyHashTable(snapshot);
    freeHashTable(snapshot);
}
