//
// Created by fiy-pc on 2024/10/2.
//

#ifndef DISKIO_H
#define DISKIO_H
#include <stdio.h>
#include "SDS.h"
#include "DataSet.h"
#include "Hash.h"


SDS odbget(HashTable *ht,SDS key);
SDS odbdelete(HashTable *ht,SDS key);
SDS odbsetSDS(HashTable *ht,SDS key , SDS value);
SDS odbsave(HashTable *ht,const char *filename);
SDS odbrgsave(HashTable *ht,const char *filename);
void odbLoad(HashTable *ht,char *filename);
void odbclose(FILE *odb);

#endif //DISKIO_H
