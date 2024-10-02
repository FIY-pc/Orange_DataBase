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

void* command_to_byte(SDS *sds);
SDSData sdsWrap(SDS sds);

#endif //COMMAND_PARSER_H
