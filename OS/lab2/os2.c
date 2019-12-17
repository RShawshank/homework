#include<pthread.h>
#include<stdio.h>
#include<linux/sem.h>//包含创建信号量
#include<sys/errno.h>
#define PRINT 0//信号量集编号
#define COUNT 1//同上
int sum=0;
 int sem_id=0;
int P(int sem_id,int sem_num)
{
    struct sembuf sem_b;
    sem_b.sem_num=sem_num;
    sem_b.sem_op=-1;//p   -1
    sem_b.sem_flg=SEM_UNDO;//操作系统跟踪当前进程对该信号量进行修改
    if(semop(sem_id,&sem_b,1)==-1)//nsops==1，表示操作的元素个数为1
    {
        return -1;
    }
    return 0;
}
int V(int sem_id,int sem_num)
{
    struct sembuf sem_b;
    sem_b.sem_num=sem_num;
    sem_b.sem_op=1;
    sem_b.sem_flg=SEM_UNDO;//操作系统跟踪当前进程对该信号量进行修改
    if(semop(sem_id,&sem_b,1)==-1)
    {
        return -1;
    }
    return 0;
}
//打印线程
void* print(void *arg)
{
    for(int i=1;i<=100;i++)
    {
        if(P(sem_id,PRINT)==-1)
            printf("p操作执行失败\n");
            printf("The sum is :%d\n",sum);
            if(V(sem_id,COUNT)==-1)
            printf("v操作执行失败\n");
    }
    return NULL;
}
void* count(void *arg)
{
    for(int i=1;i<=100;i++)
    {
        if(P(sem_id,COUNT)==-1)
            printf("p操作执行失败\n");
            sum+=i;
            if(V(sem_id,PRINT)==-1)
            printf("v操作执行失败\n");
    }
    return NULL;
}
int INIT_SEM_COUNT(int sem_id,int sem_num)
{
	union semun sem;
	sem.val=1;
	return semctl(sem_id,sem_num,SETVAL,sem);
}
int INIT_SEM_PRINT(int sem_id,int sem_num)
{
	union semun sem;
	sem.val=0;
	int a=semctl(sem_id,sem_num,SETVAL,sem);
	//printf("%s",errno);
	return a;
}

int main(void)
{
    pthread_t thread[2];//线程号 0表示print，1表示count
    //创建信号量集，IPC_PRIVAIE意味着key==0 不能被不相关的进程操控,IPC_CREAT标示创建新的信号量
    if((sem_id = semget(0,2,IPC_CREAT|0666))==-1)
        printf("创建失败\n");
        //信号量初始化，并编号为val
        if(INIT_SEM_PRINT(sem_id,PRINT)==-1)
        printf("初始化打印信号量失败\n");
        if(INIT_SEM_COUNT(sem_id,COUNT)==-1)
        printf("初始化计数信号量失败\n");
        //创建线程:第一个参数为指向线程标识符的指针；第三个参数为线程运行函数地址
        if(pthread_create(&thread[0],NULL,count,NULL)!=0)
        printf("创建线程失败！\n");
        if(pthread_create(&thread[1],NULL,print,NULL)!=0)
        printf("创建线程失败！\n");
        pthread_join(thread[0],NULL);
        pthread_join(thread[1],NULL);
        //删除信号量集
        if(semctl(sem_id,IPC_RMID,0)==-1)
        printf("删除失败\n");
    return 0;
}
