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

SDSData sdsWrap(SDS sds);
CommandType getCommandType(const SDS *method);
int command_cheek(SDS *command);
void* command_to_byte(SDS *sds);
void splitCommand(const SDS *command, SDS *method, SDS *params);
int splitParams(const SDS params, char output[][MAX_PARAM_LEN], int maxParams);

#endif //COMMAND_PARSER_H
