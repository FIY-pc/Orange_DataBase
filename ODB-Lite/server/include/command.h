//
// Created by fiy-pc on 2024/10/6.
//

#ifndef COMMAND_H
#define COMMAND_H
#include "SDS.h"
#include "Realizer.h"

SDS command_executor(HashTable *ht,SDS command);
int commands_handler(HashTable *ht,int clientfd,SDS *commands);
#endif //COMMAND_H
