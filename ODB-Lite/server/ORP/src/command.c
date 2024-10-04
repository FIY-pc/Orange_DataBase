#include "command.h"
#include "ORPSET.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>



// 检查命令格式
int command_check(const SDS *command)
{
    // printf("begin command check\n");
    SDS method,paramsline;
    SDS params[MAX_PARAM_NUM];

    // 初步切分方法与参数，进行参数数量过筛
    splitCommand(command,&method,&paramsline);
    if(sds_len(&paramsline)==0)
    {
        printf("invalid params\n");
        sds_free(&paramsline);
        sds_free(&method);
        return -1;
    }

    // printf("params num valid\n");
    // 方法类型过筛
    const CommandType type = getCommandType(&method);
    if(type==COMMAND_UNKNOWN)
    {
        printf("unknown method\n");
        return -1;
    }

    // printf("method type:%d\n",type);
    // 方法参数量过筛
    int paramCount = splitParams(paramsline,params,MAX_PARAM_NUM);
    if(paramCount<=0)
    {
        printf("params split fail\n");
        return -1;
    }
    // printf("params num valid\n");
    printf("params num is %d\n",paramCount);
    switch(type)
    {
        case COMMAND_GET:
            if(paramCount!=1)
            {
                printf("invalid params num\n");
                return -1;
            }
            break;
        case COMMAND_SET:
            if(paramCount!=2)
            {
                printf("invalid params num\n");
                return -1;
            }
            break;
        case COMMAND_DELETE:
            if(paramCount!=1)
            {
                printf("invalid params num\n");
                return -1;
            }
            break;
        default:
    }
    sds_free(&method);
    sds_free(&paramsline);
    // printf("command check success\n");
    return 0;
}

// 返回根据从命令中切割出来的方法字符串生成的枚举值
CommandType getCommandType(const SDS *method) {
    if (strcasecmp(method->data, "set") == 0) {
        return COMMAND_SET;
    }
    if (strcasecmp(method->data, "get") == 0) {
        return COMMAND_GET;
    }
    if (strcasecmp(method->data, "delete") == 0) {
        return COMMAND_DELETE;
    }
    if (strcasecmp(method->data, "save") == 0)
    {
        return COMMAND_SAVE;
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
    // printf("begin split\n");
    const char *open_paren = strchr((*command).data, '(');
    if (open_paren == NULL) {
        *method = *command;  // 如果没有找到'(', 则整个字符串是命令
        *params = (SDS){0, NULL};  // 参数为空
        return;
    }
    // 计算命令的长度
    const int command_len = open_paren - (*command).data;
    // printf("length:%d\n",command_len);
    // 为命令分配内存并复制
    *method = sds_new((*command).data);
    method->data[command_len] = '\0';  // 终止命令字符串
    // 为参数分配内存并复制（包括括号）
    *params = sds_new(open_paren);
    // printf("split command success\n");
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
