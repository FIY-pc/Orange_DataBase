#include "SDS-ORP.h"
#include <stdlib.h>


SDSData sdsWrap(SDS sds)
{
    SDSData sdsd;
    sdsd.type = SDSTYPE;
    sdsd.sds = sds;
    return sdsd;
}

void* serialize_SDSData(SDSData *data, size_t *out_len) {
    size_t total_len = sizeof(char) + sizeof(int32_t) + data->sds.len; // type, len, data
    char *buffer = (char *)malloc(total_len);

    if (!buffer) return NULL;

    char *ptr = buffer;

    // 序列化 type
    *ptr++ = data->type;

    // 序列化 sds.len
    int32_t len = data->sds.len; // 使用 int32_t 保证 4 字节长度
    // 复制 len 的四个字节
    *ptr++ = (len >> 24) & 0xFF;
    *ptr++ = (len >> 16) & 0xFF;
    *ptr++ = (len >> 8) & 0xFF;
    *ptr++ = len & 0xFF;

    // 序列化 sds.data
    for (size_t i = 0; i < data->sds.len; i++) {
        *ptr++ = data->sds.data[i];
    }

    *out_len = total_len;
    return buffer;
}

SDSData* deserialize_SDSData(const void *buffer, size_t len) {
    const char *ptr = (const char *)buffer;

    if (len < sizeof(char) + sizeof(int32_t)) return NULL;

    SDSData *data = (SDSData *)malloc(sizeof(SDSData));
    if (!data) return NULL;

    // 反序列化 type
    data->type = *ptr++;

    // 反序列化 sds.len
    int32_t sds_len = 0;
    sds_len |= (*ptr++ & 0xFF) << 24;
    sds_len |= (*ptr++ & 0xFF) << 16;
    sds_len |= (*ptr++ & 0xFF) << 8;
    sds_len |= (*ptr++ & 0xFF);

    if (len - (ptr - (const char *)buffer) < (size_t)sds_len) {
        free(data);
        return NULL;
    }

    // 反序列化 sds.data
    data->sds.len = sds_len;
    data->sds.data = (char *)malloc(sds_len + 1);
    if (!data->sds.data) {
        free(data);
        return NULL;
    }

    for (int32_t i = 0; i < sds_len; i++) {
        data->sds.data[i] = *ptr++;
    }
    data->sds.data[sds_len] = '\0';

    return data;
}