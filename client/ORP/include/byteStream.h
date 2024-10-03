//
// Created by fiy-pc on 2024/10/2.
//

#ifndef BYTESTREAM_H
#define BYTESTREAM_H
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    void *data;
    u_int32_t len;
} ByteStream;

// 组合任意数量字节流包，生成拼接后的字节流
ByteStream byteJoin(const ByteStream *bytestreams, const u_int32_t count);
#endif //BYTESTREAM_H
