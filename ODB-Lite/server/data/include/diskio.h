//
// Created by fiy-pc on 2024/10/2.
//

#ifndef DISKIO_H
#define DISKIO_H
#include <stdio.h>
#include "SDS.h"


SDS odbget(SDS key);
SDS odbdelete(SDS key);
SDS odbset(SDS key , SDS value);
FILE *odbopen();
void odbclose(FILE *odb);

#endif //DISKIO_H
