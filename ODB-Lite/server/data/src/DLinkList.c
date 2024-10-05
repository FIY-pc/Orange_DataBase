//
// Created by fiy-pc on 2024/10/5.
//

#include "DLinkList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 假设你已经定义了SDS结构体和DLinkListNode结构体

// 创建一个新的DLinkListNode
DLinkListNode* createDLinkListNode(const char* str) {
    DLinkListNode* newNode = (DLinkListNode*)malloc(sizeof(DLinkListNode));
    if (!newNode) return NULL;

    newNode->data.len = strlen(str);
    newNode->data.data = (char*)malloc(newNode->data.len + 1);
    if (!newNode->data.data) {
        free(newNode);
        return NULL;
    }
    strcpy(newNode->data.data, str);

    newNode->prev = NULL;
    newNode->next = NULL;

    return newNode;
}

// 向链表的头部添加节点
void addToHead(DLinkListNode** head, DLinkListNode* newNode) {
    if (*head == NULL) {
        *head = newNode;
    } else {
        newNode->next = *head;
        (*head)->prev = newNode;
        *head = newNode;
    }
}

// 向链表的尾部添加节点
void addToTail(DLinkListNode** head, DLinkListNode* newNode) {
    if (*head == NULL) {
        *head = newNode;
    } else {
        DLinkListNode* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
        newNode->prev = temp;
    }
}

// 删除指定节点
void deleteNode(DLinkListNode** head, DLinkListNode* delNode) {
    if (*head == NULL || delNode == NULL) return;

    if (*head == delNode) {
        *head = delNode->next;
    }
    if (delNode->next != NULL) {
        delNode->next->prev = delNode->prev;
    }
    if (delNode->prev != NULL) {
        delNode->prev->next = delNode->next;
    }

    free(delNode->data.data);
    free(delNode);
}

// 查找包含指定字符串的节点
DLinkListNode* findNode(DLinkListNode* head, const char* str) {
    DLinkListNode* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->data.data, str) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// 打印链表中的所有节点
void printList(DLinkListNode* head) {
    DLinkListNode* temp = head;
    while (temp != NULL) {
        printf("%s\n", temp->data.data);
        temp = temp->next;
    }
}

// 销毁整个链表
void destroyList(DLinkListNode** head) {
    DLinkListNode* temp;
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next;
        free(temp->data.data);
        free(temp);
    }
}
