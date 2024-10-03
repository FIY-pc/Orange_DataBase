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
void* command_to_byte(const SDS *sds);
void* serialize_RQStream(RQStream *rqstream, u_int32_t *out_len);
void print_hex(const unsigned char *data, const u_int32_t size);
void splitCommand(const SDS *command, SDS *method, SDS *params);
int command_check(const SDS *command);
int splitParams(const SDS params, char output[][MAX_PARAM_LEN], const int maxParams);

#endif //COMMAND_PARSER_H
