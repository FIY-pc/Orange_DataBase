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

// 命令种类
typedef enum {
    COMMAND_SET,
    COMMAND_GET,
    COMMAND_DELETE,
    COMMAND_SAVE,
    COMMAND_RGSAVE,
    COMMAND_AUTOSAVE,
    COMMAND_ADDR,
    COMMAND_ADDL,
    COMMAND_LINDEX,
    COMMAND_HSET,
    COMMAND_HGET,
    COMMAND_HDEL,
    COMMAND_SADD,
    COMMAND_SMEMBERS,
    COMMAND_UNKNOWN
} CommandType;

CommandType getCommandType(const SDS *method);
void splitCommand(const SDS *command, SDS *method, SDS *params);
int command_check(const SDS *command);
int splitParams(const SDS params, SDS output[], const int maxParams);

#endif //COMMAND_PARSER_H
