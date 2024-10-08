//
// Created by fiy-pc on 2024/10/6.
//

#ifndef TRANSMIT_H
#define TRANSMIT_H

char *dataPack(char **data, int linesNum);
char** dataUnpack(char *pack, int *outputLinesNum);
int readlines(int fd, char ***data);
void writelines(int fd, char **data,int linesNum);

#endif //TRANSMIT_H
