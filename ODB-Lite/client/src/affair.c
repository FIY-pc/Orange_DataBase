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
    char affair[MAX_AFFAIR_SIZE][MAX_LINE_LEN];
    int affairNum = 0;

    for (int i=0; i<MAX_AFFAIR_SIZE; i++)
    {
        strcpy(buffer,"");
        fgets(buffer, BUFSIZ, stdin);
        if (strncmp(buffer, "commit", 6) == 0)
        {
            break;
        }
        if (strncmp(buffer, "reset", 5) == 0)
        {
            return;
        }
        buffer[strcspn(buffer, "\n")] = 0;

        strcpy(affair[i], buffer);
        affairNum++;
    }
    // 将所有命令发送
    writelines(sockfdClient,affair,affairNum);

    printf("affair commit success!\n");

    // 接收所有响应
    char response[MAX_LINES_NUM][MAX_LINE_LEN];
    int responseNum = readlines(sockfdClient,response);
    for(int i=0;i<responseNum;i++)
    {
        printf("%s\n",response[i]);
    }
}