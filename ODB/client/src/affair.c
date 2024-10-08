//
// Created by fiy-pc on 2024/10/4.
//

#include "affair.h"

#include <stdio.h>
#include <string.h>
#include <transmit.h>
#include <unistd.h>
#include <sys/socket.h>

#include "ORPSET.h"

void affair(int sockfdClient)
{
    char buffer[BUFSIZ];

    // 事务包
    char *affair[MAX_AFFAIR_SIZE];
    // 添加开始标志
    int affairNum = 1;
    affair[0] = strdup("begin");
    printf("affair begin\n");
    for (int i=1; i<MAX_AFFAIR_SIZE; i++)
    {
        strcpy(buffer,"");
        fgets(buffer, BUFSIZ, stdin);
        if (strncmp(buffer, "commit", 6) == 0)
        {
            break;
        }
        if (strncmp(buffer, "reset", 5) == 0)
        {
            printf("affair reset\n");
            return;
        }
        buffer[strcspn(buffer, "\n")] = 0;

        affair[i] = strdup(buffer);
        affairNum++;
    }
    // 将所有命令发送
    writelines(sockfdClient,affair,affairNum);

    printf("affair commit success!\n");

    // 接收所有响应
    char **response;
    int responseNum = readlines(sockfdClient,&response);
    for(int i=0;i<responseNum;i++)
    {
        printf("%s\n",response[i]);
    }
}