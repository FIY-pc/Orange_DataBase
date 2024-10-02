//
// Created by fiy-pc on 2024/10/2.
//

#ifndef SDS_ORP_H
#define SDS_ORP_H

#include "SDS.h"
#include "ORPSET.h"

//将sds包装成SDSData数据包结构体
SDSData sdsWrap(SDS sds);
//SDS序列化函数
void* serialize_SDSData(SDSData *data, size_t *out_len);
//SDS反序列化函数
SDSData* deserialize_SDSData(const void *buffer, size_t len);
#endif //SDS_ORP_H
