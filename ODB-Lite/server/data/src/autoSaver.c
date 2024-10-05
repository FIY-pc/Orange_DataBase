#include "autoSaver.h"
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

// 定义全局变量
int change_count = 0;
int autoSaverSwitch = 0;

pthread_mutex_t change_count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t autoSaverSwitch_mutex = PTHREAD_MUTEX_INITIALIZER;

void increment_change_count() {
    pthread_mutex_lock(&change_count_mutex);
    change_count++;
    pthread_mutex_unlock(&change_count_mutex);
}

void reset_change_count() {
    pthread_mutex_lock(&change_count_mutex);
    change_count = 0;
    pthread_mutex_unlock(&change_count_mutex);
}

int get_change_count() {
    int count;
    pthread_mutex_lock(&change_count_mutex);
    count = change_count;
    pthread_mutex_unlock(&change_count_mutex);
    return count;
}

void open_autoSaver() {
    pthread_mutex_lock(&autoSaverSwitch_mutex);
    autoSaverSwitch = 1;
    pthread_mutex_unlock(&autoSaverSwitch_mutex);
}

void close_autoSaver() {
    pthread_mutex_lock(&autoSaverSwitch_mutex);
    autoSaverSwitch = 0;
    pthread_mutex_unlock(&autoSaverSwitch_mutex);
}

void *autoSaver(void *arg) {
    autoSaverArgs *args = (autoSaverArgs *)arg;
    int time, changeNum;
    sscanf(args->time.data, "%d", &time);
    sscanf(args->changeNum.data, "%d", &changeNum);

    while (1) {
        pthread_mutex_lock(&autoSaverSwitch_mutex);
        if (autoSaverSwitch == 0) {
            pthread_mutex_unlock(&autoSaverSwitch_mutex);
            break;
        }
        pthread_mutex_unlock(&autoSaverSwitch_mutex);

        if (get_change_count() >= changeNum) {
            odbsave(args->ht, args->fileName);
            reset_change_count();
        }
        sleep(time);
    }

    // 清理动态分配的内存，如果SDS类型需要手动释放
    sds_free(&args->time);
    sds_free(&args->changeNum);
    free(args); // 释放args内存

    return NULL;
}

SDS autoSaver_create(HashTable *ht, const char *filename, SDS time, SDS changeNum) {
    if (isValidPositiveInteger(time.data) == 0 || isValidPositiveInteger(changeNum.data) == 0) {
        printf("autoSaver create failed due to illegal param\n");
        return sds_new("autoSaver create failed due to illegal param");
    }
    if (strcmp(time.data, "0") == 0 || strcmp(changeNum.data, "0") == 0) {
        return sds_new("autoSaver not be created because auto saving is closed");
    }

    open_autoSaver();
    autoSaverArgs *args = malloc(sizeof(autoSaverArgs));
    if (args == NULL) {
        printf("Memory allocation failed\n");
        return sds_new("Memory allocation failed");
    }

    args->ht = ht;
    args->time = time; // 确保这些SDS不会在函数外部被释放或修改
    args->changeNum = changeNum;
    strncpy(args->fileName, filename, sizeof(args->fileName) - 1);
    args->fileName[sizeof(args->fileName) - 1] = '\0';

    pthread_t autoSaver_thread;
    if (pthread_create(&autoSaver_thread, NULL, autoSaver, (void *)args) != 0) {
        free(args); // 创建线程失败，释放args
        return sds_new("Failed to create autoSaver thread");
    }

    // 分离线程，以便线程自动清理
    pthread_detach(autoSaver_thread);

    return sds_new("autoSaver created");
}