#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <arpa/inet.h>

#include "config.h"

int main()
{
    int sockfd;
    struct sockaddr_in server;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT);


    while(1)
    {
        int sockAccept = accept(sockfd, (struct sockaddr *)&server, (socklen_t *)&server);
        if(sockAccept == -1)
        {
            continue;
        }

    }
}