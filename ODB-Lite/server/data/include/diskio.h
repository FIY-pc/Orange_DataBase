//
// Created by fiy-pc on 2024/10/2.
//

#ifndef DISKIO_H
#define DISKIO_H
#include <stdio.h>
#include "SDS.h"
#include "DataSet.h"
#include "Hash.h"


SDS odbget(SDS key);
SDS odbdelete(SDS key);
SDS odbset(SDS key , SDS value);
void odbLoad(HashTable *ht,char *filename);
void odbclose(FILE *odb);

#endif //DISKIO_H
