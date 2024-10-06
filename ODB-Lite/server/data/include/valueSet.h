//
// Created by fiy-pc on 2024/10/5.
//

#ifndef SET_H
#define SET_H

#include "valueHash.h"

SDS valueSet_to_SDS(valueHash *valueSet);
valueHash *SDS_to_valueSet(SDS formatSet);
#endif //SET_H
