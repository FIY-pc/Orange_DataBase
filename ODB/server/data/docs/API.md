# API(data)

[TOC]



## Realizer(命令实现总接口)

```c
// 常规命令
SDS odbget(HashTable *ht,SDS key);
SDS odbdelete(HashTable *ht,SDS key);
SDS odbsetSDS(HashTable *ht,SDS key , SDS value);
// 保存命令
SDS odbsave(HashTable *ht,const char *filename);
SDS odbrgsave(HashTable *ht,const char *filename);
SDS odbautosave(HashTable *ht,const char *filename,SDS time,SDS changeNum);
// 列表命令
SDS odbaddr(HashTable *ht,SDS key , SDS value);
SDS odbaddl(HashTable *ht,SDS key , SDS value);
SDS odblindex(HashTable *ht,SDS key,SDS index);
// 哈希表命令
SDS odbhset(HashTable *ht,SDS key,SDS field,SDS value);
SDS odbhget(HashTable *ht,SDS key,SDS field);
SDS odbhdel(HashTable *ht,SDS key,SDS field);
// 集合命令
SDS odbsadd(HashTable *ht,SDS key,SDS value);
SDS odbsmembers(HashTable *ht,SDS key);
// 加载函数
void odbLoad(HashTable *ht,char *filename);
// 辅助函数
int isValidNaturalInteger(const char *str);//用于判断一个字符串的内容是否是自然数
```



## 底层实现

### Hash(数据库存储核心)

定义了用于数据库存储的哈希表，其定义如下：

```c
typedef struct Entry {
    char key[MAX_KEY_LEN];
    char value[MAX_VALUE_LEN];
    struct Entry *next;
    int is_modified;   //用于标记条目是否改变
    int is_loaded;  // 用于标记条目是否从文件中加载
    int is_in_hash_table; // 用于标记条目是否在哈希表中
} Entry;

typedef struct HashTable {
    Entry *entries[HASH_TABLE_INIT_SIZE];
} HashTable;
```

```c
void hashInit(HashTable *ht);
// 创建一个新的哈希表

unsigned int hash_function(const char *key);
// djb2哈希函数

void hashSet(HashTable *ht, const char *key, const char *value);
const char *hashGet(HashTable *ht, const char *key);
void hashDelete(HashTable *ht, const char *key);
// 哈希表基本操作

void freeHashTable(HashTable *ht);
// 释放哈希表

void hash_save_to_file(HashTable *ht, const char *filename);
// 用于将哈希表保存到文件中
void hash_load_from_file(HashTable *ht, const char *filename);
// 用于从文件加载哈希表

HashTable *copyHashTable(const HashTable *src);
// 完整地复制一个独立的哈希表并返回
void replaceHashTable(HashTable **original, HashTable *replacement);
// 用replacement覆盖original

int isFileEmpty(const char *filename);
// 辅助函数，用于从文件加载哈希表时判断文件是否为空
```



### autoSaver(自动保存模块)

定义了与自动保存有关的三个外部变量以及操作这些变量的线程安全函数，还有数据变化监视器的线程创建与线程函数，以及传参包。

```c
// 用外部变量记录变化数
extern int change_count;
// 用外部变量控制线程关闭
extern int autoSaverSwitch;
extern double saveTime;
// 操作变化数的函数
void increment_change_count();
void reset_change_count();
int get_change_count();

// 操作计时的函数
void increment_Time();
void reset_Time();
double get_Time();

// 操作变化监视器线程开关的函数
void open_autoSaver();
void close_autoSaver();


// 参数包
typedef struct
{
    HashTable *ht;
    SDS time;
    SDS changeNum;
    char fileName[256];
}autoSaverArgs;

// 变化监视器线程相关函数
SDS autoSaver_create(HashTable *ht, const char *filename,SDS time,SDS changeNum);
void *autoSaver(void *arg);
```



### SDS(简单字符串)

提供简单字符串的定义与基本操作API

```c
typedef struct
{
    int len;
    char *data;
}SDS;

SDS sds_new(const char *str);
SDS sds_cpy(const SDS sds);
void sds_free(SDS *sds);
void sds_set(SDS *sds, const char *data) ;
void sds_print(SDS *sds);
void printSDSArray(SDS *arr, int size);
void freeSDSArray(SDS *arr, int size);
int sds_cmp(const SDS sds1, const SDS sds2);
int sds_len(SDS *sds);
char *sds_get(const SDS *sds);
char* strcat_sds(SDS *sds, SDS *add);
```

### DLinkList(双向列表的定义与基本函数)

支撑列表valueList的实现

```c
typedef struct DLinkListNode{
    SDS data;
    struct DLinkListNode * prev;
    struct DLinkListNode * next;
}DLinkListNode;

DLinkListNode* createDLinkListNode(const char* str);
void addToHead(DLinkListNode** head, DLinkListNode* newNode);
void addToTail(DLinkListNode** head, DLinkListNode* newNode);
void deleteNode(DLinkListNode** head, DLinkListNode* delNode);
void destroyList(DLinkListNode** head);
void printList(DLinkListNode* head);
DLinkListNode* findNode(DLinkListNode* head, const char* str);
SDS getNodeSDSByIndex(DLinkListNode* head, int index);
```



### valueList(列表解释器)

提供格式化字符串形式列表与双向链表形式列表相互转换的API，依赖DLinkList

```c
DLinkListNode *SDS_to_list(SDS formatList);
SDS list_to_SDS(DLinkListNode *head);
```

### valueHash(哈希解释器)

提供作为值的正常哈希表定义与基本函数，格式化字符串形式哈希表与哈希表相互转换的API

```c
#define VALUE_HASH_INIT_SIZE 9973

typedef struct valueEntry
{
    char key[MAX_KEY_LEN];
    char value[MAX_VALUE_LEN];
    struct valueEntry *next;
}valueEntry;

typedef struct valueHash
{
    valueEntry *valueEntries[VALUE_HASH_INIT_SIZE];
}valueHash;


const char *valueHashGet(valueHash *vht, const char *key);
void valueHashSet(valueHash *vht,const char *key,const char *value);
void valueHashDelete(valueHash *vht, const char *key);
// 一般哈希表定义与基本函数

valueHash *SDS_to_valueHash(SDS formatHash);
SDS valueHash_to_SDS(valueHash *vht);

// 哈希格式化与解析函数
```



### valueSet(集合解释器)

提供集合格式化函数与解析函数，依赖valueHash

```c
SDS valueSet_to_SDS(valueHash *valueSet);
valueHash *SDS_to_valueSet(SDS formatSet);
```

