//
// Created by fiy-pc on 2024/10/2.
//

#include "diskio.h"

#include <stdlib.h>


void odbLoad(HashTable *ht,char *filename)
{
    hash_load_from_file(ht,filename);
    printf("ODB Loaded\n");
}

SDS odbget(SDS key)
{
    SDS message = sds_new("");
    return message;
}
SDS odbdelete(SDS key)
{
    SDS message = sds_new("");
    return message;
}
SDS odbset(SDS key , SDS value)
{
    SDS message = sds_new("");
    return message;
}