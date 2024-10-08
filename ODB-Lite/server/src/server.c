#include <affair.h>
#include <autoSaver.h>
#include <ORPSET.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <transmit.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "config.h"
#include "SDS.h"
#include "Realizer.h"

pthread_mutex_t ht_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct
{
    HashTable **ppht;
    int clientfd;
}connection_args;

void run_server();
void *connect_handler(void *args);

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

        // 打包连接线程参数
        connection_args *args = malloc(sizeof(connection_args));
        args->ppht = &ht;
        args->clientfd = clientfd;

        // 建立连接线程
        pthread_t connection_thread;
        if (pthread_create(&connection_thread, NULL, connect_handler, (void *)args) != 0) {
            free(args); // 创建线程失败，释放args
            perror("connection_thread create fail!\n");
        }
        pthread_detach(connection_thread);
    }
}


void *connect_handler(void *args) {
    connection_args *params = (connection_args *)args;
    HashTable **ht = params->ppht;
    int clientfd = params->clientfd;

    while (1)
    {
        // autocommit
        int autoCommitSwitch = 0;
        pthread_mutex_lock(&ht_mutex);        // 为ht加锁
        const char *autocommit = hashGet(*ht,ODB_SETTING_AUTOCOMMIT); // 段错误
        if (autocommit == NULL)
        {
            perror("hashGet autocommit setting fail!\n");
        }
        else if(strncmp(autocommit,"true",strlen("true")) == 0)
        {
            printf("Autocommit enabled\n");
            autoCommitSwitch = 1;
        }
        else if (strncmp(autocommit,"false",strlen("false")) == 0)
        {
            printf("Autocommit disabled\n");
            autoCommitSwitch = 0;
        }
        else
        {
            perror("Autocommit setting wrong!\n");
            printf("autocommit setting now set to 'false'\n");
            hashSet(*ht,ODB_SETTING_AUTOCOMMIT,"false");
        }
        pthread_mutex_unlock(&ht_mutex); // ht解锁
        SDS affair[MAX_AFFAIR_SIZE]={0,NULL};

        // 请求存包
        char **request;
        int lines = readlines(clientfd,&request);
        if (lines <= 0)
        {
            perror("readlines fail!\n");
            close(clientfd);
            return NULL;
        }
        // 命令打包
        printf("lines = %d\n", lines);
        for (int i = 0; i < lines; i++)
        {
            affair[i] = sds_new(request[i]);
        }
        // 事务处理
        affair_handler(ht,clientfd,affair,autoCommitSwitch);

        // autoSaver reopen(待优化)
        autoSaverSwitch = 0;
        usleep(2000);
        autoSaverSwitch = 1;
        odbautosave(*ht,ODB_FILE_DIR,odbget(*ht,sds_new(ODB_SETTING_AUTOSAVE_TIME)),odbget(*ht,sds_new(ODB_SETTING_AUTOSAVE_CHANGENUM)));
    }
}