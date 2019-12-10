#include <stdio.h>      //标准IO头文件
#include <stdlib.h>     /* Standard library */
#include <unistd.h>     //exex、fork头文件
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#define BUF_SIZE 30//

int pid1, pid2;



void childProcess_handler(int signal) {
    kill(pid1, SIGUSR1);
    kill(pid2, SIGUSR2);
}


void sigusr_handler(int signal) {
    if (signal == SIGUSR1) {
        printf("Child Process 1 is Killed by Parent!\n");
        exit(0);
    }
    else if (signal == SIGUSR2) {
        printf("Child Process 2 is Killed by Parent!\n");
        exit(0);
    }
}

int main(void) {
    int pfd[2];//定义两个
    char read_buf[BUF_SIZE];//输出缓存区
    int read_num;
    int wait_tmp;
    signal(SIGINT, childProcess_handler);

//当进程创建管道时，每次
//都需要提供两个文件描述符来操作管道。其中一个对管道进行写操作，另一个对管道进行读操作。对管道的读写与一般的IO系统函数一
//致，使用write()函数写入数据，使用read()读出数据。
//成功，返回0，否则返回-1。参数数组包含pipe使用的两个文件的描述符。pfd[0]:读管道，pfd[1]:写管道。
    if (pipe(pfd) == -1)
        printf("fail to create pipe\n");
    /**
     *当调用fork()时，将执行以下动作：
      向系统申请一个新PID
      创建子进程，复制父进程的PCB，获得父进程的数据空间、堆、栈等资源的副本
      在父进程中返回子进程的PID，在子进程中返回0
     *
     */
    switch (pid1 = fork()) {
        case -1:
            printf("First fork create fail\n");

            /* Child 1 for write data */
        case 0:
            /*SIG_ING 代表忽略SIGINT信号 */
            signal(SIGINT, SIG_IGN);
            /*捕捉SIGUSR1*/
            signal(SIGUSR1, sigusr_handler);
            if (close(pfd[0]) == -1)//子进程1关闭读端
                printf("Child 1 close read fd fail\n");

            int count = 1;
            char write_buf[BUF_SIZE];
            while(1) {
                sprintf(write_buf, "I send you %d times.\n", count);
                if (write(pfd[1], write_buf, strlen(write_buf)) != strlen(write_buf))
                {
                    printf("Child 1 write fail/n");
                }
                sleep(1);
                count++;
            }

            /* 父进程 */
        default:
            switch (pid2 = fork()) {
                case -1:
                    printf("Second fork create fail\n");
                    /* Child 2 for read data */
                case 0:
                    signal(SIGINT, SIG_IGN);
                    signal(SIGUSR2, sigusr_handler);
                    if (close(pfd[1] == -1))
                        printf("Child 2 close write fd fail\n");

                    while(1) {
                        read_num = read(pfd[0], read_buf, BUF_SIZE);
                        if (read_num == -1)
                            printf("Child 2 read fail\n");
                        if (read_num == 0)
                            continue;
                        if (write(STDOUT_FILENO, read_buf, read_num) != read_num)
                            printf("Child 2 write fail\n");
                    }
                default:
                    if (close(pfd[0]) == -1)
                        printf("Parent close pfd[0]\n");
                    if (close(pfd[1]) == -1)
                        printf("Parent close pfd[1]\n");
                    waitpid(pid1, &wait_tmp, 0);
                    waitpid(pid1, &wait_tmp, 0);
                    printf("Parent Process is Killed!\n");
                    exit(0);
            }
    }
}