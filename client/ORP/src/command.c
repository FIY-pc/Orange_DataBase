#include "command.h"
#include "SDS-ORP.h"
#include "ORPSET.h"
#include "byteStream.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void* command_to_byte(SDS *command)
{
    // 检查
    if (command == NULL)
    {
        return NULL;
    }
    if(command_cheek(command) == -1)
    {
        printf("command_to_byte error\n");
        return NULL;
    }

    // 处理命令
    SDS method,params;
    char output[MAX_PARAM_NUM][MAX_PARAM_LEN];

    splitCommand(command,&method,&params);
    CommandType type = getCommandType(&method);
    int paramCount = splitParams(params,output,MAX_PARAM_NUM);
    if (paramCount == -1)
    {
        printf("command_to_byte error\n");
        return NULL;
    }

    // 将SDS参数加入标识信息打包成SDSData包，再序列化为字节流
    SDSData SDSParamPack[paramCount];
    u_int32_t SDSParamPackLen[paramCount];
    for (int i = 0; i < paramCount; i++)
    {
        SDSParamPack[i] = sdsWrap(sds_new(output[i]));
    }
    void *SDSParamByte[paramCount];
    for (int i = 0; i < paramCount; i++)
    {
        SDSParamByte[i] = serialize_SDSData(SDSParamPack+i,SDSParamPackLen+i);
    }
    // 拼接所有参数数据包字节流


    // 加入头部信息，打包成最终请求数据包RQStream



    sds_free(&method);
    sds_free(&params);
    return NULL;
}

int command_cheek(SDS *command)
{
    SDS method,params;
    char output[MAX_PARAM_NUM][MAX_PARAM_LEN];

    // 初步切分方法与参数，进行参数数量过筛
    splitCommand(command,&method,&params);
    if(sds_len(&params)==0)
    {
        printf("invalid params\n");
        sds_free(&params);
        sds_free(&method);
        return -1;
    }
    // 方法类型过筛
    CommandType type = getCommandType(&method);
    if(type==COMMAND_UNKNOWN)
    {
        printf("unknown method\n");
        return -1;
    }

    // 方法参数量过筛
    int paramCount = splitParams(params,output,MAX_PARAM_NUM);
    if(paramCount<0)
    {
        printf("params split fail\n");
        return -1;
    }
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
    sds_free(&params);
    return 0;
}

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
    return COMMAND_UNKNOWN;
}

// 打印字节流，用于测试
void print_hex(const unsigned char *data, u_int32_t size)
{
    for (u_int32_t i = 0; i < size; ++i) {
        printf("%02x ", data[i]);
    }
    printf("\n");
}

void splitCommand(const SDS *command, SDS *method, SDS *params) {
    /*
     * command : 整条SDS类型命令
     * method : 输出方法
     * params : 输出带括号参数串
     */
    // 查找第一个'('的位置
    char *open_paren = strchr((*command).data, '(');
    if (open_paren == NULL) {
        *method = *command;  // 如果没有找到'(', 则整个字符串是命令
        *params = (SDS){0, NULL};  // 参数为空
        return;
    }
    // 计算命令的长度
    int command_len = open_paren - (*command).data;
    // 为命令分配内存并复制
    if(method->data != NULL)
    {
        sds_free(method);
    }
    *method = sds_new((*command).data);
    method->data[command_len] = '\0';  // 终止命令字符串
    // 为参数分配内存并复制（包括括号）
    *params = sds_new(open_paren);
}


int splitParams(const SDS params, char output[][MAX_PARAM_LEN], int maxParams) {
    /*  params : splitCommand中切割出的(参数1,参数2...)
     *  output : 存放切割出来的各参数
     *  maxParams : 最大获取参数数量
     */
    int paramCount = 0;
    const char *token;
    char *tempStr;
    u_int32_t tempStrLen;

    // 为了避免直接修改 params.data，复制一份到临时字符串
    tempStrLen = params.len + 1;
    tempStr = (char *)malloc(tempStrLen * sizeof(char));
    if (tempStr == NULL) {
        // 内存分配失败
        return -1;
    }
    strncpy(tempStr, params.data, tempStrLen - 1);
    tempStr[tempStrLen - 1] = '\0'; // 确保字符串以 '\0' 结尾

    token = strtok(tempStr, ",");
    while (token != NULL && paramCount < maxParams) {
        strncpy(output[paramCount], token, MAX_PARAM_LEN - 1);
        output[paramCount][MAX_PARAM_LEN - 1] = '\0'; // 确保参数字符串以 '\0' 结尾
        paramCount++;
        token = strtok(NULL, ",");
    }

    // 释放临时字符串的内存
    free(tempStr);
    return paramCount;
}
