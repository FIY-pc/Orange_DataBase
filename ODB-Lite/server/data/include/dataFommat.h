//
// Created by fiy-pc on 2024/10/4.
//

#ifndef DATAPARSER_H
#define DATAPARSER_H
#include "SDS.h"

SDS format_sds(const SDS *sds);
SDS format_number(int num);
SDS format_list(SDS *list, int size);
SDS parse_sds(const char *str);
int parse_number(const char *str);
SDS* parse_list(const char *str, int *size);
void free_sds_list(SDS *list, int size);
#endif //DATAPARSER_H
