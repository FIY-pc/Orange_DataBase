//
// Created by fiy-pc on 2024/10/1.
//

#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H
#include "ORPSET.h"
struct Command
{
    char *method;
    void *params;
};

CommandType getCommandType(const SDS *method);
void splitCommand(const SDS *command, SDS *method, SDS *params);
int command_check(const SDS *command);
int splitParams(const SDS params, SDS output[], const int maxParams);

#endif //COMMAND_PARSER_H
