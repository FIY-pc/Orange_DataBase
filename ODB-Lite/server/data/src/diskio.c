//
// Created by fiy-pc on 2024/10/2.
//

#include "diskio.h"

#include <ctype.h>

#include "autoSaver.h"

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
    if(isValidPositiveInteger(time.data)==0 || isValidPositiveInteger(changeNum.data)==0)
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
        usleep(5000);
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

int isValidPositiveInteger(const char *str) {
    if (str == NULL || *str == '\0') {
        return 0;
    }
    if (*str == '0' && *(str + 1) != '\0') {
        return 0;
    }
    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}