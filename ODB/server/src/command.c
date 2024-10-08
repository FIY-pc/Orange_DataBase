//
// Created by fiy-pc on 2024/10/6.
//

#include <stdio.h>

#include "command.h"
#include "command_parser.h"
#include "Hash.h"
#include "Realizer.h"

#include <config.h>
#include <string.h>
#include <transmit.h>
#include <unistd.h>


int commands_handler(HashTable *ht,int clientfd,SDS *commands)
{
    int commandOrdinal = 0;

    SDS *current_command = commands;
    char *response[MAX_LINES_NUM];
    int responseIndex = 0;

    char rawresponse[MAX_LINE_LEN]; // 用于暂存加工过程中的一条响应

    // 统计总命令条数
    int totalCommands = 0;
    while(current_command->data != NULL)
    {
        totalCommands++;
        current_command++;
    }

    current_command = commands;
    while (current_command->data)
    {
        // 命令解析
        if (command_check(current_command) == -1) {
            printf("commands_handler: Command not found\n");
            snprintf(rawresponse,sizeof(rawresponse),"Execute affair failed, %d commands input , %d success",totalCommands,commandOrdinal);
            char *temp[1];
            temp[0] = strdup(rawresponse);
            writelines(clientfd,temp,1);
            return commandOrdinal;
        }

        // 执行命令并保存响应信息
        SDS message = command_executor(ht,*current_command);
        sprintf(rawresponse, "OK,%s",message.data);
        response[responseIndex] = strdup(rawresponse);

        // 清空rawresponse
        memset(rawresponse, 0, MAX_LINE_LEN);
        responseIndex++;
        current_command++;
        commandOrdinal++;
    }

    snprintf(rawresponse,sizeof(rawresponse),"Execute affair success, %d commands input , %d success",totalCommands,commandOrdinal);
    response[responseIndex] = strdup(rawresponse);
    responseIndex++;
    writelines(clientfd,response,responseIndex);
    return commandOrdinal;
}

SDS command_executor(HashTable *ht,SDS command)
{
    SDS message = sds_new("");
    SDS method, paramsline;
    splitCommand(&command, &method, &paramsline);
    printf("command_executor/splitCommand\n"); //
    CommandType commandType = getCommandType(&method);
    printf("command_executor/getCommandType\n"); //
    SDS params[MAX_PARAM_NUM];
    splitParams(paramsline, params,MAX_PARAM_NUM);
    printf("command_executor/splitParams\n"); //
    // 命令执行
    switch (commandType)
    {
    case COMMAND_GET:
        printf("command_executor/commandget\n"); //
        message = odbget(ht, params[0]);
        break;
    case COMMAND_SET:
        printf("command_executor/commandset\n"); //
        message = odbsetSDS(ht, params[0], params[1]);
        break;
    case COMMAND_DELETE:
        printf("command_executor/commanddelete\n"); //
        message = odbdelete(ht, params[0]);
        break;
    case COMMAND_SAVE:
        printf("command_executor/commandsave\n");
        message = odbsave(ht,ODB_FILE_DIR);
        break;
    case COMMAND_RGSAVE:
        printf("command_executor/commandrgsave\n");
        message = odbrgsave(ht,ODB_FILE_DIR);
        break;
    case COMMAND_AUTOSAVE:
        printf("command_executor/commandautosave\n");
        message = odbautosave(ht,ODB_FILE_DIR, params[0], params[1]);
        break;
    case COMMAND_ADDR:
        printf("command_executor/commandaddr\n");
        message = odbaddr(ht, params[0], params[1]);
        break;
    case COMMAND_ADDL:
        printf("command_executor/commandaddl\n");
        message = odbaddl(ht, params[0], params[1]);
        break;
    case COMMAND_LINDEX:
        printf("command_executor/commandlindex\n");
        message = odblindex(ht, params[0], params[1]);
        break;
    case COMMAND_HSET:
        printf("command_executor/commandhset\n");
        message = odbhset(ht, params[0], params[1], params[2]);
        break;
    case COMMAND_HGET:
        printf("command_executor/commandhget\n");
        message = odbhget(ht, params[0], params[1]);
        break;
    case COMMAND_HDEL:
        printf("command_executor/commandhdel\n");
        message = odbhdel(ht, params[0], params[1]);
        break;
    case COMMAND_SADD:
        printf("command_executor/commandsadd\n");
        message = odbsadd(ht, params[0], params[1]);
        break;
    case COMMAND_SMEMBERS:
        printf("command_executor/commandsmembers\n");
        message = odbsmembers(ht, params[0]);
        break;
    default:
        message = sds_new("execute ERROR");
    }
    return message;
}