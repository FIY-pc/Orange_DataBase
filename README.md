# Orange_DataBase
[TOC]



## 简介

这是一个简单的NoSQL键值对数据库，在Linux环境下运作，包含一套服务端和客户端，通过socket和自带的orange通信协议进行命令的传输，目前能正常实现字符串、列表、哈希表的存储以及事务、并发。

## 安装

(以下以Ubuntu环境为示例)

在release里面下载最新版的ODB-Server-1.x.x-desktop.zip，解压之后

，文件夹里面有两个文件夹：client和server

## 使用

### 事务

使用set(autocommit,true/false)来控制是否开启自动事务提交。

**请注意：**默认**不开启**自动事务提交，此情况下若要提交命令，必须先输入begin开启一项事务，输完所有命令后再输入commit命令进行事务提交。**直接输入命令并回车将会得到illegal Input**



### 基本命令

set(key,value)设置一个键值对

get(key)查询某个键对应的值

delete(key)删除某个键值对



### 保存数据

#### 手动保存

使用SAVE命令生成快照(不推荐，该命令会阻塞服务端)

使用RGSAVE命令创建一个子进程进行快照生成(推荐，不会阻塞服务端)

#### 自动保存

使用save(time,changeNum)来设置自动快照生成，功能为每满time秒检查一次数据更改，若发生了changeNum次更改，则进行一次快照生成。

若要关闭此功能，可将任意一个参数设为0



### 列表

#### 创建列表

通过类似set(key,[value1;value2])的命令创建一个列表

#### 操作列表

addr(key,value)	若key对应的值是一个列表，则在列表右侧插入一个值。

addl(key,value)	类似addr，但是是在列表左侧插入值

lindex(key,index)	若key对应的值是一个列表，则查询列表从左向右序号为index的数据(序号从0开始)



### 哈希

#### 创建哈希表

通过类似set(key,{key1:value1;key2:value2})的命令创建一个哈希表

**请注意**：作为值的哈希表中的key和value用`:`分隔，两个键值对之间用`;`分隔



#### 操作哈希表

hset(key,field,value)	若key对应的值是一个哈希表，则在该哈希表中设置一个键值对field:value

hget(key,field)	若key对应的值是一个哈希表，则查询该哈希表中field键对应的值

hdel(key,field)	若key对应的值是一个哈希表，则删除该哈希表中field对应的键值对



### 集合

#### 创建集合

通过类似set(key,(value1;value2)的命令创建一个集合

#### 操作集合

sadd(key,value)	若key对应的值是一个集合，且value在集合中不存在，则在集合中添加value

smembers(key)	显示key对应的集合中的所有值



## 开发者指南

