#include <command_parser.h>

#include "command.h"
#include "ORPSET.h"
#include <string.h>
#include <stdio.h>



// 检查命令格式
int command_check(const SDS *command)
{
    SDS method,paramsline;
    SDS params[MAX_PARAM_NUM];

    // 初步切分方法与参数，进行参数数量过筛
    splitCommand(command,&method,&paramsline);
    printf("method is %s\n",method.data);
    printf("paramsline is %s\n",paramsline.data);
    const CommandType type = getCommandType(&method);
    printf("type is %d\n",type);
    // 若是检测到无参数命令，进行进一步检查
    if(strcmp(paramsline.data,"()")==0||strcmp(paramsline.data,"")==0)
    {
        switch(type)
        {
            case COMMAND_SAVE:
                return 0;
            case COMMAND_RGSAVE:
                return 0;
            default:
                printf("command type not found(empty params command)\n");
                return -1;
        }
    }

    // 方法类型过筛
    if(type==COMMAND_UNKNOWN)
    {
        printf("unknown method\n");
        return -1;
    }

    // 方法参数量过筛
    int paramCount = splitParams(paramsline,params,MAX_PARAM_NUM);
    if(paramCount<=0)
    {
        printf("params split fail\n");
        return -1;
    }
    // printf("params num valid\n");
    printf("params num is %d\n",paramCount);
    switch (type)
    {
    case COMMAND_GET:
        if (paramCount != 1)
        {
            printf("invalid params num\n");
            return -1;
        }
        break;
    case COMMAND_SET:
        if (paramCount != 2)
        {
            printf("invalid params num\n");
            return -1;
        }
        break;
    case COMMAND_DELETE:
        if (paramCount != 1)
        {
            printf("invalid params num\n");
            return -1;
        }
        break;
    case COMMAND_AUTOSAVE:
        if (paramCount != 2)
        {
            printf("invalid params num\n");
            return -1;
        }
        break;
    case COMMAND_ADDR:
        if (paramCount != 2)
        {
            printf("invalid params num\n");
            return -1;
        }
        break;
    case COMMAND_ADDL:
        if (paramCount != 2)
        {
            printf("invalid params num\n");
            return -1;
        }
        break;
    case COMMAND_LINDEX:
        if (paramCount != 2)
        {
            printf("invalid params num\n");
            return -1;
        }
        break;
    case COMMAND_HSET:
        if (paramCount != 3)
        {
            printf("invalid params num\n");
            return -1;
        }
        break;
    case COMMAND_HGET:
        if (paramCount != 2)
        {
            printf("invalid params num\n");
            return -1;
        }
        break;
    case COMMAND_HDEL:
        if (paramCount != 2)
        {
            printf("invalid params num\n");
            return -1;
        }
        break;
    case COMMAND_SADD:
        if (paramCount != 2)
        {
            printf("invalid params num\n");
            return -1;
        }
        break;
    case COMMAND_SMEMBERS:
        if (paramCount != 1)
        {
            printf("invalid params num\n");
            return -1;
        }
        break;
    default:
    }
    sds_free(&method);
    sds_free(&paramsline);
    return 0;
}

// 返回方法字符串对应的枚举值
CommandType getCommandType(const SDS *method) {
    printf("getCommandType: method is %s\n",method->data);
    printf("getCommandType: methodlen is %d\n",method->len);
    if (strcasecmp(method->data, "set") == 0) {
        return COMMAND_SET;
    }
    if (strcasecmp(method->data, "get") == 0) {
        return COMMAND_GET;
    }
    if (strcasecmp(method->data, "delete") == 0) {
        return COMMAND_DELETE;
    }
    if (strncmp(method->data, "SAVE",4) == 0){
        return COMMAND_SAVE;
    }
    if (strncmp(method->data, "RGSAVE",6) == 0)
    {
        return COMMAND_RGSAVE;
    }
    if(strncmp(method->data,"save",4) == 0)
    {
        return COMMAND_AUTOSAVE;
    }
    if(strncmp(method->data,"addr",4) == 0)
    {
        return COMMAND_ADDR;
    }
    if(strncmp(method->data,"addl",4) == 0)
    {
        return COMMAND_ADDL;
    }
    if(strncmp(method->data,"lindex",6) == 0)
    {
        return COMMAND_LINDEX;
    }
    if (strncmp(method->data,"hset",4) == 0)
    {
        return COMMAND_HSET;
    }
    if (strncmp(method->data,"hget",4) == 0)
    {
        return COMMAND_HGET;
    }
    if (strncmp(method->data,"hdel",4) == 0)
    {
        return COMMAND_HDEL;
    }
    if (strncmp(method->data,"sadd",4) == 0)
    {
        return COMMAND_SADD;
    }
    if (strncmp(method->data,"smembers",8) == 0)
    {
        return COMMAND_SMEMBERS;
    }
    return COMMAND_UNKNOWN;
}

    /*
     * command : 整条SDS类型命令
     * method : 输出方法
     * params : 输出带括号参数串
     */
void splitCommand(const SDS *command, SDS *method, SDS *params) {
    // 查找第一个'('的位置
    const char *open_paren = strchr((*command).data, '(');
    if (open_paren == NULL) {
        *method = sds_cpy(*command);  // 复制整个命令
        *params = (SDS){0, ""};  // 参数为空
        return;
    }
    // 计算命令的长度
    const int command_len = open_paren - (*command).data;
    // 为命令分配内存并复制
    *method = sds_new((*command).data);
    method->data[command_len] = '\0';  // 终止命令字符串
    method->len = command_len;  // 更新长度
    // 为参数分配内存并复制（包括括号）
    *params = sds_new(open_paren);
}

/*
 *  将参数从带括号参数行切割出来
 *  params : 带括号参数行
 *  output : 用于接收参数的SDS数组
 *  maxParams : 规定最大读取参数数量
 *  return : 切割所得参数数量
 */
int splitParams(const SDS params, SDS output[], const int maxParams) {
    int paramCount = 0;
    // 避免直接修改 params.data，复制一份到临时字符串
    char* tempStr = sds_get(&params);
    if (tempStr == NULL || *tempStr == '\0' || (*tempStr == '(' && *(tempStr + 1) == ')')) {
        // 输入为空或仅为空括号
        return 0;
    }
    // 如果输入包含括号，则去掉括号
    char* start = (tempStr[0] == '(') ? tempStr + 1 : tempStr;
    char* end = strrchr(start, ')');
    if (end != NULL) {
        *end = '\0';
    }
    const char* token = strtok(start, ",");
    while (token != NULL && paramCount < maxParams) {
        output[paramCount] = sds_new(token);
        paramCount++;
        token = strtok(NULL, ",");
    }
    return paramCount;
}
