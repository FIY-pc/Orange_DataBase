//
// Created by fiy-pc on 2024/10/2.
//

#ifndef DISKIO_H
#define DISKIO_H

#include "SDS.h"

#include "Hash.h"

// ODB命令实现API
// 常规命令
SDS odbget(HashTable *ht,SDS key);
SDS odbdelete(HashTable *ht,SDS key);
SDS odbsetSDS(HashTable *ht,SDS key , SDS value);
// 保存命令
SDS odbsave(HashTable *ht,const char *filename);
SDS odbrgsave(HashTable *ht,const char *filename);
SDS odbautosave(HashTable *ht,const char *filename,SDS time,SDS changeNum);
// 列表命令
SDS odbaddr(HashTable *ht,SDS key , SDS value);
SDS odbaddl(HashTable *ht,SDS key , SDS value);
SDS odblindex(HashTable *ht,SDS key,SDS index);
// 哈希表命令
SDS odbhset(HashTable *ht,SDS key,SDS field,SDS value);
SDS odbhget(HashTable *ht,SDS key,SDS field);
SDS odbhdel(HashTable *ht,SDS key,SDS field);

// 加载函数
void odbLoad(HashTable *ht,char *filename);
// 辅助函数
int isValidNaturalInteger(const char *str);
#endif //DISKIO_H
