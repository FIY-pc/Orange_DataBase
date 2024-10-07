#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "ORPSET.h"
#include "transmit.h"
#include "config.h"
#include "affair.h"

int main(){
    int sockfd;
    //建立包含服务端IP与端口信息的sockaddr结构体
    struct sockaddr_in server;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(IP);
    server.sin_port = htons(PORT);

    //连接服务端
    if(connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect fail!\nPress enter to exit\n");
        getchar();
        exit(EXIT_FAILURE);
    }
    printf("connected to server\n");


    //收发数据
     while(1)
     {
         char buffer[BUFSIZ] = {0};
         // 输入命令并进行检查
         fgets(buffer,BUFSIZ, stdin);
         buffer[strcspn(buffer,"\n")]=0;
         if(strncmp(buffer,"close",5) == 0)
         {
             close(sockfd);
             printf("client closed\nPress enter to exit\n");
             getchar();
             return 0;
         }
         if(strncmp(buffer,"begin",5) == 0)
         {
             affair(sockfd);
             continue;
         }
         // 自动事务提交，一命令一事务
         char request[2][MAX_LINE_LEN];
         strncpy(request[0], buffer, strlen(buffer)+1);
         strncpy(request[1], "commit", strlen("commit")+1);

         writelines(sockfd, request,2);

         printf("commited\n");
         char response[MAX_LINES_NUM][MAX_LINES_NUM];
         int responseNum = readlines(sockfd,response);
         for(int i=0;i<responseNum;i++)
         {
             printf("%s\n",response[i]);
         }
    }
}
