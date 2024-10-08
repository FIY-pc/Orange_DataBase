# API(ORP)

[TOC]

## 简介

orange协议(简称ORP)是为ODB数据库设计的极简的通信协议

## 目录内容

### ORPSET(传输规定)

### transmit(传输函数)

```c
char *dataPack(char **data, int linesNum);
char** dataUnpack(char *pack, int *outputLinesNum);
// 多行数据的打包与解包函数，是多行收发的基础

int readlines(int fd, char ***data);
void writelines(int fd, char **data,int linesNum);
// 多行收发函数,防黏包
```

