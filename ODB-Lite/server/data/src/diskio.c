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
        return message;
    }
    sds_set(&message,rawData);
    return message;
}
SDS odbdelete(HashTable *ht, SDS key) {
    SDS message = sds_new("");
    const char *value = hashGet(ht, key.data);
    hashDelete(ht, key.data);
    char temp[256];
    snprintf(temp, sizeof(temp), "delete %s=%s success", key.data, value ? value : "null");
    sds_set(&message, temp);
    return message;
}
SDS odbsetSDS(HashTable *ht,SDS key , SDS value)
{
    hashSet(ht,key.data,value.data);
    printf("ODB Set end\n");
    char rawmessage[256];
    sprintf(rawmessage,"set %s=%s success",key.data,hashGet(ht,key.data));
    printf(":%s",rawmessage);
    SDS message = sds_new(rawmessage);
    return message;
}

SDS odbsave(HashTable *ht)
{

}