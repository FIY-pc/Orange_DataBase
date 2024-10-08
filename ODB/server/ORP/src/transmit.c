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
    unsigned long totalLen = 0;
    for (int i = 0; i < linesNum; i++) {
        if (data[i] == NULL) return NULL;
        totalLen = totalLen + strlen(data[i]);
    }
    totalLen += (MAX_LINES_NUM_BYTES + 1) + linesNum; // 4位数字、冒号、至少linesNum个换行符
    char *pack = malloc(totalLen + 1); // +1 for null terminator
    if (!pack) return NULL;
    snprintf(pack, totalLen + 1, "%4d:", linesNum);
    unsigned long offset = 5;
    for (int i = 0; i < linesNum; i++) {
        snprintf(pack + offset, totalLen - offset + 1, "%s\n", data[i]);
        offset += strlen(data[i]) + 1; // +1 for newline
    }
    return pack;
}

char** dataUnpack(char *pack, int *outputLinesNum) {
    if (!pack || !outputLinesNum) return NULL;
    *outputLinesNum = strtol(pack, NULL, 10);
    if (*outputLinesNum <= 0 || pack[4] != ':') return NULL;
    char **data = malloc(*outputLinesNum * sizeof(char *));
    if (!data) return NULL;
    char *temp = pack + 5;
    char *lineStart = temp;
    char *lineEnd;
    int i = 0;
    while (*temp != '\0' && i < *outputLinesNum) {
        lineEnd = strchr(lineStart, '\n');
        if (!lineEnd) {
            for (int j = 0; j < i; j++) free(data[j]);
            free(data);
            *outputLinesNum = 0;
            return NULL;
        }
        data[i] = malloc((lineEnd - lineStart + 1) * sizeof(char));
        if (!data[i]) {
            for (int j = 0; j < i; j++) free(data[j]);
            free(data);
            *outputLinesNum = 0;
            return NULL;
        }
        strncpy(data[i], lineStart, lineEnd - lineStart);
        data[i][lineEnd - lineStart] = '\0';
        lineStart = lineEnd + 1;
        i++;
    }
    if (i != *outputLinesNum) {
        for (int j = 0; j < i; j++) free(data[j]);
        free(data);
        *outputLinesNum = 0;
        return NULL;
    }
    return data;
}
// data： 一种传参方法，定义一个char *str[MAX_LINES_NUM]，传入&str
int readlines(int fd, char ***data) {
    int linesNum = 0;
    char pack[MAX_LINES_NUM * MAX_LINES_NUM_BYTES] = "";
    ssize_t bytes = read(fd, pack, sizeof(pack));
    if (bytes == -1) {
        perror("readlines: read error\n");
        return 0;
    }
    pack[bytes] = '\0';
    *data = dataUnpack(pack, &linesNum);
    if (*data == NULL) return 0;
    return linesNum;
}

void writelines(int fd, char **data,int linesNum)
{
    if(!data || !linesNum) return;
    char *pack = dataPack(data,linesNum);
    if(pack==NULL) return;
    write(fd, pack, strlen(pack));
    free(pack);
}