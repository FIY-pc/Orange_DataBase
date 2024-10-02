#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <arpa/inet.h>

#include "config.h"

int main()
{
    int sockfd;
    struct sockaddr_in server;
    // 建立套接字句柄
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(IP);
    server.sin_port = htons(PORT);

    // 绑定IP与端口
    if(bind(sockfd, (struct sockaddr *) &server, sizeof(server))==-1)
    {
        perror("bind fail!\n");
        exit(EXIT_FAILURE);
    };

    // 监听端口
    if (listen(sockfd, 5) == -1)
    {
        perror("listen fail!\n");
        exit(EXIT_FAILURE);
    }
    printf("Server started\n");

    //接受请求
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    while(1)
    {
        int clientfd = accept(sockfd, (struct sockaddr *) &client, &len);
        if(clientfd < 0)
        {
            continue;
        }
        printf("Client connected\n");

        //收发字节流
        char buffer[BUFSIZ];
        ssize_t bytes;
        bytes = read(clientfd, buffer, BUFSIZ);
        if(bytes > 0)
        {
            write(STDOUT_FILENO, buffer, bytes);
        }
    }
}