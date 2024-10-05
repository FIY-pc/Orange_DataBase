//
// Created by fiy-pc on 2024/10/2.
//
#include "DataSet.h"
#include "diskio.h"

#include "autoSaver.h"
#include "list.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void odbLoad(HashTable *ht,char *filename)
{
    hash_load_from_file(ht,filename);
    const char *autoSaveTime = hashGet(ht,ODB_SETTING_AUTOSAVE_TIME);
    if(!autoSaveTime)
    {
        hashSet(ht, ODB_SETTING_AUTOSAVE_TIME,"0");
    }else
    {
        printf("Auto saving time: %s \n",autoSaveTime);
    }
    const char *autoSaveChangeNum = hashGet(ht,ODB_SETTING_AUTOSAVE_CHANGENUM);
    if(!autoSaveChangeNum)
    {
        hashSet(ht, ODB_SETTING_AUTOSAVE_CHANGENUM,"0");
    }
    else
    {
        printf("Auto saving change num: %s \n",autoSaveChangeNum);
    }
    odbsave(ht,filename);
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

    // autoSaver
    increment_change_count();

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

    // autoSaver
    increment_change_count();

    return message;
}

SDS odbsave(HashTable *ht,const char *filename)
{
    printf("ODB Save begin\n");
    hash_save_to_file(ht,filename);
    char temp[256];
    snprintf(temp, sizeof(temp), "ODB save to %s", filename);
    printf("ODB Save end\n");
    SDS message = sds_new(temp);
    return message;
}

SDS odbrgsave(HashTable *ht,const char *filename)
{
    printf("ODB rgsave begin\n");
    pid_t pid = fork();
    if(pid<0)
    {
        perror("fork failed\n");
        return sds_new("rgsave failed");
    }
    if(pid == 0)
    {
        hash_save_to_file(ht,filename);
        exit(0);
    }
    printf("ODB rgsave end\n");
    return sds_new("rgsave success");
}

SDS odbautosave(HashTable *ht,const char *filename,SDS time,SDS changeNum)
{
    if(isValidNaturalInteger(time.data)==0 || isValidNaturalInteger(changeNum.data)==0)
    {
        printf("ODB autosave failed due to illegal param\n");
        return sds_new("illegal param");
    }
    if(strcmp(time.data,"0")==0||strcmp(changeNum.data,"0")==0)
    {
        close_autoSaver();
        printf("ODB autosave close\n");
        return sds_new("ODB autosave close");
    }
    if(autoSaverSwitch==1)
    {
        close_autoSaver();
        usleep(2000);
        open_autoSaver();
        autoSaver_create(ht,filename,time,changeNum);
        printf("ODB autosave REOPEN\n");

        // 保存配置信息
        hashSet(ht,ODB_SETTING_AUTOSAVE_TIME,time.data);
        hashSet(ht,ODB_SETTING_AUTOSAVE_CHANGENUM,changeNum.data);
        printf("ODB autosave setting saved\n");

        char rawmessage[256];
        snprintf(rawmessage, sizeof(rawmessage), "ODB autosave REOPEN!\nSave when %s changes\nEvery %s seconds run a check",changeNum.data,time.data);
        return sds_new(rawmessage);
    }
    open_autoSaver();
    autoSaver_create(ht,filename,time,changeNum);
    printf("ODB autosave OPEN\n");

    // 保存配置信息
    hashSet(ht,ODB_SETTING_AUTOSAVE_TIME,time.data);
    hashSet(ht,ODB_SETTING_AUTOSAVE_CHANGENUM,changeNum.data);
    printf("ODB autosave setting saved\n");

    char rawmessage[256];
    snprintf(rawmessage, sizeof(rawmessage), "ODB autosave OPEN!\nSave when %s changes\nEvery %s seconds run a check",changeNum.data,time.data);
    return sds_new(rawmessage);
}

SDS odbaddr(HashTable *ht,SDS key , SDS value)
{
    SDS message = sds_new("");

    SDS formatList = sds_new(hashGet(ht,key.data));
    DLinkListNode *ListHead = SDS_to_list(formatList);
    if(ListHead == NULL)
    {
        sds_set(&message,"invalid list");
        return message;
    }
    addToTail(&ListHead,createDLinkListNode(value.data));
    SDS resultFormat = list_to_SDS(ListHead);
    hashSet(ht,key.data,resultFormat.data);

    char rawmessage[256];
    snprintf(rawmessage, sizeof(rawmessage), "list change from %s to %s",formatList.data,resultFormat.data);
    sds_set(&message,rawmessage);
    printf("ODB addr success\n");

    // autoSaver
    increment_change_count();
    return message;
}

SDS odbaddl(HashTable *ht,SDS key , SDS value)
{
    SDS message = sds_new("");

    SDS formatList = sds_new(hashGet(ht,key.data));
    DLinkListNode *ListHead = SDS_to_list(formatList);
    if(ListHead == NULL)
    {
        sds_set(&message,"invalid list");
        return message;
    }
    addToHead(&ListHead,createDLinkListNode(value.data));
    SDS resultFormat = list_to_SDS(ListHead);
    hashSet(ht,key.data,resultFormat.data);

    char rawmessage[256];
    snprintf(rawmessage, sizeof(rawmessage), "list change from %s to %s",formatList.data,resultFormat.data);
    sds_set(&message,rawmessage);
    printf("ODB addl success\n");

    // autoSaver
    increment_change_count();
    return message;
}

SDS odblindex(HashTable *ht,SDS key,SDS index)
{
    SDS message = sds_new("");

    if(isValidNaturalInteger(index.data)==0)
    {
        sds_set(&message,"invalid index");
        return message;
    }
    int indexNum;
    sscanf(index.data,"%d",&indexNum);
    SDS formatList = sds_new(hashGet(ht,key.data));
    DLinkListNode *ListHead = SDS_to_list(formatList);
    if(ListHead == NULL)
    {
        sds_set(&message,"invalid list");
        return message;
    }
    SDS result = getNodeSDSByIndex(ListHead,indexNum);
    return result;
}

int isValidNaturalInteger(const char *str) {
    if (str == NULL || *str == '\0') {
        return 0;
    }
    if (*str == '0' && *(str + 1) != '\0') {
        return 1;
    }
    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}