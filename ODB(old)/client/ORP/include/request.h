//
// Created by fiy-pc on 2024/10/1.
//

#ifndef REQUEST_DEALER_H
#define REQUEST_DEALER_H

#include "ORPSET.h"
#include "byteStream.h"
typedef struct
{
    char version[HEAD_VERSION_LEN];
    char method;
    ByteStream ORPData;
}RQStream;

RQStream RQStreamPack(char method,ByteStream ORPData);
void *serialize_RQStream(RQStream *rqstream, u_int32_t *out_len);

#endif //REQUEST_DEALER_H
