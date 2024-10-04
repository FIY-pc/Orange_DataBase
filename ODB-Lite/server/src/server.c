#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "config.h"
#include "SDS.h"
#include "command.h"
#include "diskio.h"

void run_server();
void request_handler(HashTable *ht,int clientfd);

int main() {
    run_server();
    return 0;
}

void run_server() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("Current working directory: %s\n", cwd);
    }

    int sockfd;
    struct sockaddr_in server;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket fail!\n");
        getchar();
        exit(EXIT_FAILURE);
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(IP);
    server.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("bind fail!\n");
        close(sockfd);
        getchar();
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 5) == -1) {
        perror("listen fail!\n");
        close(sockfd);
        getchar();
        exit(EXIT_FAILURE);
    }

    HashTable *ht = malloc(sizeof(HashTable)); // 分配内存给哈希表
    if (ht == NULL) {
        fprintf(stderr, "Failed to allocate memory for hash table\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    hashInit(ht); // 初始化哈希表
    // 从文件加载哈希表
    odbLoad(ht, "../resources/Database.odb");

    printf("Server started!\n");

    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    while (1) {
        int clientfd = accept(sockfd, (struct sockaddr *)&client, &len);
        if (clientfd < 0) {
            perror("accept fail!\n");
            continue;
        }
        printf("Client connected\n");
        request_handler(ht,clientfd);
        close(clientfd);  // 关闭客户端连接
    }
}

void request_handler(HashTable *ht,int clientfd) {
    char request[1024];
    int bytes_received;
    SDS command;
    while ((bytes_received = recv(clientfd, request, sizeof(request) - 1, 0)) > 0) {
        request[bytes_received] = '\0';  // 确保字符串以null结尾
        command = sds_new(request);

        if (command_check(&command) == -1) {
            printf("Command not found\n");
            write(clientfd, "Invalid command\n", 15);
        } else {
            SDS message = sds_new("");
            char response[1024] = "";

            SDS method,paramsline;
            splitCommand(&command,&method,&paramsline);
            printf("server/splitCommand\n");//
            CommandType commandType = getCommandType(&method);
            printf("server/getCommandType\n");//
            SDS params[MAX_PARAM_NUM];
            splitParams(paramsline,params,MAX_PARAM_NUM);
            printf("server/splitParams\n"); //
            switch (commandType)
            {
                case COMMAND_GET:
                    printf("server/commandget\n");//
                    message = odbget(ht,params[0]);
                    break;
                case COMMAND_SET:
                    printf("server/commandset\n");//
                    message = odbsetSDS(ht,params[0],params[1]);
                    break;
                case COMMAND_DELETE:
                    printf("server/commanddelete\n");//
                    message = odbdelete(ht,params[0]);
                    break;
                case COMMAND_SAVE:
                    printf("server/commandsave\n");
                    message = odbsave(ht);
                default:
            }

            sprintf(response, "OK\n%s",message.data);
            write(clientfd,response,strlen(response)+1);
        }
    }

    if (bytes_received == 0) {
        printf("Client disconnected\n");
    } else if (bytes_received < 0) {
        perror("recv fail!\n");
    }
}