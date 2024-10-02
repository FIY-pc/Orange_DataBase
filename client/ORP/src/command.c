#include "command.h"
#include "SDS-ORP.h"
#include <stdlib.h>
#include <stdio.h>

void* command_to_byte(SDS *command)
{

    return NULL;
}

// 打印字节流，用于测试
void print_hex(const unsigned char *data, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        printf("%02x ", data[i]);
    }
    printf("\n");
}