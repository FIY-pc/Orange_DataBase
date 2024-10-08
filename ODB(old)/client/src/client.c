#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "config.h"
#include "SDS.h"
void send_data(int sockfd,SDS command);
void recv_data(int sockfd);

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
        perror("connect fail!\n");
        exit(EXIT_FAILURE);
    }
    printf("connected to server\n");

    char buffer[BUFSIZ];
    //收发数据
    while(1)
    {
        fgets(buffer,BUFSIZ, stdin);
        if(strcmp(buffer,"close") == 0)
        {
            return 0;
        }
        SDS command = sds_new(buffer);
        send_data(sockfd, command);
        recv_data(sockfd);
    }
}

void send_data(int sockfd,SDS command)
{
    char buffer[BUFSIZ];
    printf("sending data\n");

}

void recv_data(int sockfd)
{

}