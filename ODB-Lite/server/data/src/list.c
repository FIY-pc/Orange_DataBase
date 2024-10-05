//
// Created by fiy-pc on 2024/10/5.
//

#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 将双向链表格式化为SDS
SDS list_to_SDS(DLinkListNode *head) {
    // 计算所需的总长度
    int totalLen = 0;
    DLinkListNode *temp = head;
    while (temp != NULL) {
        totalLen += temp->data.len + (temp->next != NULL ? 1 : 0); // 加上分号的长度，除了最后一个元素
        temp = temp->next;
    }
    totalLen += 2; // 加上左右方括号的长度

    // 分配内存并构建SDS
    SDS result = {0, NULL};
    result.data = (char *)calloc(totalLen + 1, sizeof(char)); // +1 为了最后的null终止符
    if (!result.data) {
        fprintf(stderr, "Memory allocation failed\n");
        return result;
    }
    result.len = totalLen;

    char *cursor = result.data;
    *cursor++ = '['; // 开括号
    temp = head;
    while (temp != NULL) {
        // 复制当前节点的数据
        strcpy(cursor, temp->data.data);
        cursor += temp->data.len;

        if (temp->next != NULL) {
            // 不是最后一个节点，添加分号
            *cursor++ = ';';
        }
        temp = temp->next;
    }
    *cursor++ = ']'; // 闭括号
    *cursor = '\0'; // null终止符

    return result;
}

// 将SDS解析为双向链表
DLinkListNode *SDS_to_list(SDS formatList) {
    if (formatList.len < 2 || formatList.data[0] != '[' || formatList.data[formatList.len - 1] != ']') {
        fprintf(stderr, "Invalid SDS format\n");
        return NULL;
    }

    DLinkListNode *head = NULL;
    DLinkListNode *tail = NULL;
    char *cursor = formatList.data + 1; // 跳过开括号
    char *end = formatList.data + formatList.len - 1; // 指向闭括号

    while (cursor < end) {
        // 查找分号作为分隔符
        char *nextComma = strchr(cursor, ';');
        if (nextComma == NULL || nextComma > end) {
            // 没有找到分号，意味着整个剩余部分都是一个元素
            nextComma = end;
        }

        // 创建新节点并添加到链表
        int len = nextComma - cursor;
        char *str = (char *)calloc(len + 1, sizeof(char));
        if (!str) {
            fprintf(stderr, "Memory allocation failed\n");
            // 需要销毁已经创建的链表
            destroyList(&head);
            return NULL;
        }
        strncpy(str, cursor, len);
        str[len] = '\0';

        DLinkListNode *newNode = createDLinkListNode(str);
        free(str); // 不再需要临时字符串
        if (!newNode) {
            // 内存分配失败，销毁已创建的链表
            destroyList(&head);
            return NULL;
        }

        if (head == NULL) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }

        // 移动到下一个元素
        cursor = nextComma + 1;
        if (*cursor == ' ') {
            cursor++;
        }
    }

    return head;
}
