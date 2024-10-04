#include <ORPSET.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

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
        char buffer[BUFSIZ]="";
        // 输入命令并进行检查
        fgets(buffer,BUFSIZ, stdin);
        if(strcmp(buffer,"close") == 0)
        {
            close(sockfd);
            printf("client closed\nPress enter to exit\n");
            getchar();
            return 0;
        }
        if(strcmp(buffer,"begin") == 0)
        {
            affair(sockfd);
            continue;
        }
        // 常规发送命令，一命令一事务
        write(sockfd,buffer,strlen(buffer));
        char response[BUFSIZ];
        read(sockfd,response,BUFSIZ);
        printf("%s\n",response);
    }
}
