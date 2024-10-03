//
// Created by fiy-pc on 2024/10/1.
//

#ifndef RESPONSE_DEALER_H
#define RESPONSE_DEALER_H

#include "ORPSET.h"
typedef struct
{
    char version[HEAD_VERSION_LEN];
    char status[HEAD_STATUS_LEN];
    void *ORPData;
}RPStream;

#endif //RESPONSE_DEALER_H
