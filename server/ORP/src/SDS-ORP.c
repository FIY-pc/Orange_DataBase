#include "SDS-ORP.h"
#include <stdlib.h>
#include <string.h>

// 将SDS包装成SDSData包
SDSData sdsWrap(SDS sds)
{
    SDSData sdsd;
    sdsd.type = SDSTYPE;
    sdsd.sds = sds;
    return sdsd;
}

// 序列化 SDSData

void* serialize_SDSData(SDSData *data, size_t *out_len) {
    size_t total_len = sizeof(char) + sizeof(int) + data->sds.len; // type, len, data
    char *buffer = (char *)malloc(total_len);
    if (!buffer) return NULL;
    char *ptr = buffer;
    // 序列化 type
    *ptr++ = data->type;
    // 序列化 sds.len
    int32_t len = data->sds.len; // 使用 int32_t 保证 4 字节长度
    memcpy(ptr, &len, sizeof(int32_t));
    ptr += sizeof(int32_t);
    // 序列化 sds.data
    memcpy(ptr, data->sds.data, data->sds.len);
    ptr += data->sds.len;
    *out_len = total_len;
    return buffer;

}
// 反序列化 SDSData
SDSData* deserialize_SDSData(const void *buffer, size_t len) {
    const char *ptr = (const char *)buffer;
    if (len < sizeof(char) + sizeof(int32_t)) return NULL;
    SDSData *data = (SDSData *)malloc(sizeof(SDSData));
    if (!data) return NULL;
    // 反序列化 type
    data->type = *ptr++;
    // 反序列化 sds.len
    int32_t sds_len;
    memcpy(&sds_len, ptr, sizeof(int32_t));
    ptr += sizeof(int32_t);
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
    memcpy(data->sds.data, ptr, sds_len);
    data->sds.data[sds_len] = '\0';
    ptr += sds_len;
    return data;
}