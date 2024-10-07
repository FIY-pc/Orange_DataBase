#include <affair.h>
#include <ORPSET.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "config.h"
#include "SDS.h"
#include "command.h"
#include "Realizer.h"

void run_server();
void connect_handler(HashTable *ht,int clientfd);

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
    odbLoad(ht, ODB_FILE_DIR);
    // autoSaver
    odbautosave(ht,ODB_FILE_DIR,odbget(ht,sds_new(ODB_SETTING_AUTOSAVE_TIME)),odbget(ht,sds_new(ODB_SETTING_AUTOSAVE_CHANGENUM)));

    printf("Server started!\n");

    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    while (1) {

        // 接受连接请求
        int clientfd = accept(sockfd, (struct sockaddr *)&client, &len);
        if (clientfd < 0) {
            perror("accept fail!\n");
            continue;
        }
        printf("Client connected\n");
        connect_handler(ht,clientfd);
    }
}

void connect_handler(HashTable *ht,int clientfd) {
    while (1)
    {
        // autocommit
        int autoCommitSwitch = 0;
        const char *autocommit = hashGet(ht,ODB_SETTING_AUTOCOMMIT);
        if (autocommit == NULL)
        {
            perror("hashGet autocommit setting fail!\n");
        }
        else if(strncmp(autocommit,"true",strlen("ture")) == 0)
        {
            autoCommitSwitch = 1;
        }
        else if (strncmp(autocommit,"false",strlen("false")) == 0){}
        else
        {
            perror("Autocommit setting wrong!\n");
            printf("autocommit setting now set to 'false'\n");
            hashSet(ht,ODB_SETTING_AUTOCOMMIT,"false");
        }

        SDS affair[MAX_AFFAIR_SIZE]={0,NULL};
        int affairIndex = 0;

        // 处理请求
        char request[1024];
        int bytes_received;
        // 命令存包
        while ((bytes_received = recv(clientfd, request, sizeof(request) - 1, 0)) > 0) {
            request[bytes_received] = '\0';  // 确保字符串以null结尾
            printf("Request received: %s\n", request);
            if(strncmp(request,"close",strlen("close")) == 0)
            {
                printf("connect closed\n");
                close(clientfd);
                return;
            }
            if(strncmp(request,"commit",6)==0)
            {
                break;
            }
            affair[affairIndex] = sds_new(request);
            affairIndex++;
        }
        // 事务处理
        affair_handler(&ht,clientfd,affair,autoCommitSwitch);

        if (bytes_received == 0) {
            printf("Client disconnected\n");
            close(clientfd);
        } else if (bytes_received < 0) {
            perror("recv fail!\n");
            close(clientfd);
        }
    }
}

