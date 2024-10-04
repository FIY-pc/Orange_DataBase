//
// Created by fiy-pc on 2024/10/4.
//

#include "dataFommat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 格式化SDS数据
SDS format_sds(const SDS *sds) {
    char *formatted = (char *)malloc(sds->len + 12); // 为格式化字符串分配内存（考虑到整数长度和冒号、逗号）
    if (!formatted) return (SDS){0, NULL};
    snprintf(formatted, sds->len + 12, "0:%d,%s", sds->len, sds->data); // 使用%zu格式化size_t类型
    SDS result = sds_new(formatted);
    free(formatted); // 释放临时内存
    return result;
}

// 格式化数字
SDS format_number(int num) {
    char *formatted = (char *)malloc(20);
    if (!formatted) return (SDS){0, NULL};
    snprintf(formatted, 20, "1:%d", num);
    SDS result = sds_new(formatted);
    free(formatted); // 释放临时内存
    return result;
}

// 格式化列表（列表中的元素已经是格式化好的SDS）
SDS format_list(SDS *list, int size) {
    size_t total_len = 0;
    // 计算所需总长度
    for (int i = 0; i < size; i++) {
        total_len += list[i].len + (i > 0 ? 1 : 0); // 加上SDS数据的长度和分隔符的长度（除了第一个元素外）
    }
    total_len += 2; // 加上列表的类型标志和末尾的空字符预留空间（假设列表类型为2）

    char *formatted = (char *)malloc(total_len);
    if (!formatted) return (SDS){0, NULL};

    formatted[0] = '2'; // 列表类型标志
    formatted[1] = ':';
    char *cursor = formatted + 2;
    for (int i = 0; i < size; i++) {
        if (i > 0) {
            *cursor++ = ';'; // 添加分隔符（除了第一个元素外）
        }
        memcpy(cursor, list[i].data, list[i].len); // 复制SDS数据
        cursor += list[i].len;
    }
    *cursor = '\0'; // 添加字符串末尾的空字符

    SDS result = sds_new(formatted);
    free(formatted); // 释放临时内存
    return result;
}

// 解析SDS数据
SDS parse_sds(const char *str) {
    int len;
    const char *data;
    sscanf(str, "%*d,%n", &len, &data); // 跳过类型标志和长度，找到数据开始的位置
    data += strspn(data, " "); // 跳过可能存在的空格（如果有的话）
    SDS result = sds_new(data);
    result.len = len; // 设置SDS的长度（这里假设数据已经是正确的长度，不需要额外验证）
    return result;
}

// 解析数字
int parse_number(const char *str) {
    int num;
    sscanf(str, "%*d:%d", &num); // 跳过类型标志，读取数字
    return num;
}

// 解析列表（这里假设列表中的元素都是SDS类型，且已经格式化）
SDS* parse_list(const char *str, int *size) {
    // 首先计算列表中的元素数量
    int count = 0;
    const char *cursor = str + 2; // 跳过列表类型标志和冒号
    while (*cursor) {
        if (*cursor == ';') count++;
        cursor++;
    }
    count++; // 修正元素数量（最后一个元素不会以分号结尾）

    SDS *list = (SDS *)malloc(count * sizeof(SDS));
    if (!list) return NULL;

    cursor = str + 2; // 再次设置光标到列表数据的开始位置
    char *token;
    int index = 0;
    while ((token = strsep(&cursor, ";")) != NULL) {
        list[index++] = parse_sds(token); // 解析每个SDS元素
    }

    *size = count;
    return list;
}

// 辅助函数：释放SDS列表的内存
void free_sds_list(SDS *list, int size) {
    for (int i = 0; i < size; i++) {
        sds_free(&list[i]);
    }
    free(list);
}
