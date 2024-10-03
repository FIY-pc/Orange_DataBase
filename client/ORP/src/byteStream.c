//
// Created by fiy-pc on 2024/10/2.
//

#include "byteStream.h"
#include <string.h>


ByteStream byteJoin(const ByteStream *bytestreams, const u_int32_t count) {
    // 计算字节流总长度
    u_int32_t totalLength = 0;
    for (u_int32_t i = 0; i < count; ++i) {
        totalLength += bytestreams[i].len;
    }
    // 分配内存
    void *resultData = malloc(totalLength);
    if (resultData == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    // 拼接
    char *currentPos = (char *)resultData;
    for (u_int32_t i = 0; i < count; ++i) {
        memcpy(currentPos, bytestreams[i].data, bytestreams[i].len);
        currentPos += bytestreams[i].len;
    }
    // 创建并返回包含总长度信息的 ByteStream 结构体
    ByteStream result = {resultData, totalLength};
    return result;
}