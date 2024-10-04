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

SDS odbget(HashTable *ht,SDS key)
{
    SDS message = sds_new("");
    const char *rawData = hashGet(ht,key.data);
    if(rawData == NULL)
    {
        sds_set(&message,"rawData not found");
    }
    sds_set(&message,rawData);
    return message;
}
SDS odbdelete(HashTable *ht,SDS key)
{
    SDS message = sds_new("");
    char rawmessage[128] = "";
    const char *value= hashGet(ht,key.data);
    hashDelete(ht,key.data);
    sprintf(key.data,"delete %s=%s success",key.data,value);
    sds_set(&message,rawmessage);
    return message;
}
SDS odbsetSDS(HashTable *ht,SDS key , SDS value)
{
    printf("ODB Set begin\n");
    printf("ODB Set key : %s\n",key.data);
    printf("ODB Set value : %s\n",value.data);
    hashSet(ht,key.data,value.data);
    printf("ODB Set end\n");
    char rawmessage[256];
    sprintf(rawmessage,"set %s=%s success",key.data,hashGet(ht,key.data));
    printf(":%s",rawmessage);
    SDS message = sds_new(rawmessage);
    return message;
}