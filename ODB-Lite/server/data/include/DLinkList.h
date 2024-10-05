//
// Created by fiy-pc on 2024/10/5.
//

#ifndef DLINKLIST_H
#define DLINKLIST_H

#include "SDS.h"

typedef struct DLinkListNode{
    SDS data;
    struct DLinkListNode * prev;
    struct DLinkListNode * next;
}DLinkListNode;

DLinkListNode* createDLinkListNode(const char* str);
void addToHead(DLinkListNode** head, DLinkListNode* newNode);
void addToTail(DLinkListNode** head, DLinkListNode* newNode);
void deleteNode(DLinkListNode** head, DLinkListNode* delNode);
void destroyList(DLinkListNode** head);
void printList(DLinkListNode* head);
DLinkListNode* findNode(DLinkListNode* head, const char* str);
SDS getNodeSDSByIndex(DLinkListNode* head, int index);

#endif //DLINKLIST_H
