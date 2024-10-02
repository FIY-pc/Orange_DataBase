#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <arpa/inet.h>

#include "config.h"

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

    //收发数据
    char buffer[BUFSIZ] = "12345";
    write(sockfd, buffer, sizeof(buffer)-1);
    read(sockfd, buffer, BUFSIZ);
    printf("%s\n", buffer);
}