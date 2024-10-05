//
// Created by fiy-pc on 2024/10/2.
//

#ifndef DISKIO_H
#define DISKIO_H

#include "SDS.h"

#include "Hash.h"

// ODB命令实现API
SDS odbget(HashTable *ht,SDS key);
SDS odbdelete(HashTable *ht,SDS key);
SDS odbsetSDS(HashTable *ht,SDS key , SDS value);
SDS odbaddr(HashTable *ht,SDS key , SDS value);
SDS odbaddl(HashTable *ht,SDS key , SDS value);
SDS odbsave(HashTable *ht,const char *filename);
SDS odbrgsave(HashTable *ht,const char *filename);
SDS odbautosave(HashTable *ht,const char *filename,SDS time,SDS changeNum);

void odbLoad(HashTable *ht,char *filename);

// 辅助函数
int isValidPositiveInteger(const char *str);
#endif //DISKIO_H
