//
// Created by fiy-pc on 2024/10/1.
//

#include "request.h"

#include <string.h>

const char* ORPGetVersion() {
    static char version[HEAD_VERSION_LEN];
    snprintf(version, sizeof(version), "%d.%d", ORP_MAJOR_VERSION, ORP_MINOR_VERSION);
    return version;
}

RQStream RQStreamPack(const char method,ByteStream ORPData)
{
    RQStream rqstream;
    strcpy(rqstream.version, ORPGetVersion());
    rqstream.method = method;
    rqstream.ORPData = ORPData;
    return rqstream;
}

