//
// Created by fiy-pc on 2024/10/6.
//

#include "transmit.h"
#include "ORPSET.h"


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *dataPack(char **data, int linesNum) {
    // 计算总长度，加上4位数字和冒号以及换行符的长度
    int totalLen = 0;
    for (int i = 0; i < linesNum; i++) {
        totalLen += strlen(data[i]);
    }
    totalLen += (MAX_LINES_NUM_BYTES + 1) + linesNum; // 4位数字、冒号、至少linesNum个换行符

    // 构建包
    char *pack = malloc(totalLen);
    if (!pack) return NULL; // 分配失败时返回NULL

    sprintf(pack, "%4d:", linesNum);
    for (int i = 0; i < linesNum; i++) {
        strcat(pack, data[i]);
        strcat(pack, "\n");
    }
    return pack;
}

void dataUnpack(char *pack, char **data, int *outputLinesNum) {
    // 首先解析行数
    if (sscanf(pack, "%4d:", outputLinesNum) != 1) {
        // 解析失败，数据不完整
        if (data) *data = NULL;
        if (outputLinesNum) *outputLinesNum = 0;
        return;
    }

    // 跳过数字和冒号
    char *temp = pack + 5; // 4位数字 + 1个冒号 = 5个字符

    // 分配内存给data数组
    data = (char **)malloc(*outputLinesNum * sizeof(char *));
    if (!data) return; // 分配失败时直接返回

    char *lineStart = temp;
    char *lineEnd;
    int i = 0;
    while (*temp != '\0' && i < *outputLinesNum) {
        // 查找行尾
        lineEnd = strchr(lineStart, '\n');
        if (!lineEnd) {
            // 没找到换行符，数据不完整
            free(data);
            data = NULL;
            if (outputLinesNum) *outputLinesNum = 0;
            return;
        }

        // 为当前行分配内存并复制内容
        data[i] = (char *)malloc((lineEnd - lineStart + 1) * sizeof(char));
        if (!data[i]) {
            // 分配失败，清理已分配的内存
            for (int j = 0; j < i; j++) free(data[j]);
            free(data);
            data = NULL;
            if (outputLinesNum) *outputLinesNum = 0;
            return;
        }
        strncpy(data[i], lineStart, lineEnd - lineStart);
        data[i][lineEnd - lineStart] = '\0'; // 确保字符串以null结尾

        // 移动到下一行
        lineStart = lineEnd + 1;
        i++;
    }

    // 检查是否读取了预期的行数
    if (i != *outputLinesNum) {
        // 数据不完整
        for (int j = 0; j < i; j++) free(data[j]);
        free(data);
        data = NULL;
        if (outputLinesNum) *outputLinesNum = 0;
    }
}

int readlines(int fd, char **data)
{
    while (1)
    {
        int linesNum = 0;
        char pack[MAX_LINES_NUM*MAX_LINES_NUM_BYTES]="";
        ssize_t bytes = read(fd,pack,sizeof(pack));
        if(bytes==-1)
        {
            perror("readlines: read error\n");
            return 0;
        }
        pack[bytes-1] = '\0';
        dataUnpack(pack,data,&linesNum);
        if(linesNum!=0)
        {
            write(fd,ORP_OK_SIGN,strlen(ORP_OK_SIGN)+1);
            return linesNum;
        }
        write(fd,ORP_ERROR_SIGN,strlen(ORP_ERROR_SIGN)+1);
    }
}

void writelines(int fd, char **data,int linesNum)
{
    if(!data || !linesNum) return;
    char *pack = dataPack(data,linesNum);

    char buffer[256];
    while (1)
    {
        write(fd,pack,strlen(pack));
        ssize_t bytes = read(fd,buffer,sizeof(buffer));
        if(bytes==-1)
        {
            perror("writelines: read error\n");
            return;
        }
        buffer[bytes-1] = '\0';
        if(strncmp(buffer,ORP_OK_SIGN,strlen(ORP_OK_SIGN)+1)==0) break;
    }
    free(pack);
}