//
// Created by fiy-pc on 2024/10/2.
//

#include "diskio.h"

FILE *odbopen()
{
    FILE *odb = fopen("resources/Database.odb", "r+");
    return odb;
}
void odbclose(FILE *odb)
{
    fclose(odb);
}

SDS odbget(SDS key)
{
    SDS message = sds_new("");
    return message;
}
SDS odbdelete(SDS key)
{
    SDS message = sds_new("");
    return message;
}
SDS odbset(SDS key , SDS value)
{
    SDS message = sds_new("");
    return message;
}