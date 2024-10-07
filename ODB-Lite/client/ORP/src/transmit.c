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
    int totalLen = 0;
    for (int i = 0; i < linesNum; i++) {
        totalLen += strlen(data[i]);
    }
    totalLen += (MAX_LINES_NUM_BYTES + 1) + linesNum; // 4位数字、冒号、至少linesNum个换行符
    char *pack = malloc(totalLen + 1); // +1 for null terminator
    if (!pack) return NULL;
    snprintf(pack, totalLen + 1, "%4d:", linesNum);
    int offset = 5; // Length of "XXXX:"
    for (int i = 0; i < linesNum; i++) {
        snprintf(pack + offset, totalLen - offset + 1, "%s\n", data[i]);
        offset += strlen(data[i]) + 1; // +1 for newline
    }
    return pack;
}

void dataUnpack(char *pack, char **data, int *outputLinesNum) {  
    if (!pack || !data || !outputLinesNum) return;
    *outputLinesNum = strtol(pack, NULL, 10);
    if (*outputLinesNum <= 0 || pack[4] != ':') return;
    char *temp = pack + 5;
    data = malloc(*outputLinesNum * sizeof(char *));
    if (!data) return;
    char *lineStart = temp;
    char *lineEnd;
    int i = 0;
    while (*temp != '\0' && i < *outputLinesNum) {
        lineEnd = strchr(lineStart, '\n');
        if (!lineEnd) {
            for (int j = 0; j < i; j++) free(data[j]);
            free(data);
            data = NULL;
            *outputLinesNum = 0;
            return;
        }
        data[i] = malloc((lineEnd - lineStart + 1) * sizeof(char));
        if (!data[i]) {
            for (int j = 0; j < i; j++) free(data[j]);
            free(data);
            data = NULL;
            *outputLinesNum = 0;
            return;
        }
        strncpy(data[i], lineStart, lineEnd - lineStart);
        data[i][lineEnd - lineStart] = '\0';
        lineStart = lineEnd + 1;
        i++;
    }
    if (i != *outputLinesNum) {
        for (int j = 0; j < i; j++) free(data[j]);
        free(data);
        data = NULL;
        *outputLinesNum = 0;
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