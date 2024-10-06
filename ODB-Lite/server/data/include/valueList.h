//
// Created by fiy-pc on 2024/10/5.
//

#ifndef LIST_H
#define LIST_H
#include "SDS.h"
#include "DLinkList.h"

DLinkListNode *SDS_to_list(SDS formatList);
SDS list_to_SDS(DLinkListNode *head);

#endif //LIST_H
