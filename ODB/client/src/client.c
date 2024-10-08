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
         char buffer[MAX_LINE_LEN];
         // 输入命令并进行检查
         fgets(buffer,MAX_LINE_LEN-1, stdin);

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

         //自动事务提交，一命令一事务
         char *request[MAX_LINES_NUM];
         request[0] = strdup(buffer);
         request[0][strcspn(request[0], "\n")] = 0;

         writelines(sockfd, request,1);
         printf("affair commit success!\n");

         char **response;
         int responseNum = readlines(sockfd,&response);
         printf("responseNum = %d\n",responseNum);
         for(int i=0;i<responseNum;i++)
         {
             if(response[i]==NULL) continue;
             printf("%s\n",response[i]);
         }
    }
}
