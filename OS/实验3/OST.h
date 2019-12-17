#ifndef OST_H
#define OST_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>//包含创建信号量
#include <sys/shm.h>
#include <sys/types.h>//包含基本数据类型，pid_t等
#include <sys/stat.h>//描述文件属性
#include <fcntl.h>//I/O
#include <unistd.h>//提供服务性的api接口，如getpid
#include <wait.h>
#define ring_buf_num 5
#define FULL 0
#define EMPTY 1
#define SHM 2
union semun
{
 int val; //信号量初始值
 struct semid_ds *buf;
 unsigned short int *array;
 struct seminfo *__buf;
};
//缓冲区节点
typedef struct ring_buf
{
    int32_t end;//缓冲区内容结尾
    int32_t size;//当前缓冲区中存放的数据的个数
    int32_t next_shm;//缓冲区的下一个共享内存标识
    char data[1024];//缓冲区中的数据
}ring_buf;
#endif
