//
// Created by fiy-pc on 2024/10/2.
//

#include "byteStream.h"
#include <string.h>


void *byteJoin(ByteStream *bytestreams, u_int32_t count) {
    // 计算字节流总长度
    u_int32_t totalLength = 0;
    for (u_int32_t i = 0; i < count; ++i) {
        totalLength += bytestreams[i].length;
    }
    // 分配内存
    void *result = malloc(totalLength);
    if (result == NULL) {
        // Handle memory allocation failure
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    // 拼接
    char *currentPos = (char *)result;
    for (u_int32_t i = 0; i < count; ++i) {
        memcpy(currentPos, bytestreams[i].data, bytestreams[i].length);
        currentPos += bytestreams[i].length;
    }
    return result;

}