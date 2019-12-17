#include"OST.h"
int writebuf(int write_fd,int shmid_head,int semid);
int readbuf(int read_fd,int shmid_tail,int semid);

ring_buf *ring_buf_temp;
//PV操作函数
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
//初始化信号量集，当缓冲区为空的时候只能写，当缓冲区为满的时候只能读
/*int INIT_SEM_FULL(int sem_id,int sem_num)
{
	union semun sem;
	sem.val=0;
	return semctl(sem_id,sem_num,SETVAL,sem);
}*/
int INIT_SEM_EMPTY(int sem_id,int sem_num,int val)
{
	union semun sem;
	sem.val=val;
    return semctl(sem_id,sem_num,SETVAL,sem);
}
//锁
int INIT_SEM_SHM(int sem_id,int sem_num)
{
	union semun sem;
	sem.val=0;
	return semctl(sem_id,sem_num,SETVAL,sem);
}
int main(int argc ,char **argv)
{
//打开文件
int shm_head=0;//共享内存标识
int sem_id=0;//信号量集
int read_fd,write_fd;
read_fd=open("read.txt",O_RDWR|O_CREAT);
write_fd=open("write.txt",O_RDWR|O_CREAT,S_IRUSR|S_IWUSR|S_IXUSR);


//创建共享变量，赋给头节点
    if((shm_head=shmget(IPC_PRIVATE,sizeof(ring_buf),IPC_CREAT|0666))==-1)
    printf("创建共享内存失败\n");
    //连接共享内存到当前进程的地址空间
    ring_buf *ring_buf_temp=NULL;
    if((ring_buf_temp=(ring_buf*)shmat(shm_head,0,0))==(void*)-1)
    printf("连接失败\n");
    //创建环形缓冲
    for(int i=0;i<ring_buf_num;i++)
    {
        int newshm_id;
        if((newshm_id=shmget(IPC_PRIVATE,sizeof(ring_buf),IPC_CREAT|0666))==-1)
        {
                printf("创建共享内存失败\n");
        }
        ring_buf_temp->next_shm = newshm_id;
        ring_buf_temp->end=0;
        if((ring_buf_temp=(ring_buf*)shmat(newshm_id,0,0))==(void*)-1)
        printf("连接失败\n");
    }
    ring_buf_temp->next_shm=shm_head;
    //创建信号灯
    if((sem_id=semget(IPC_PRIVATE,3,IPC_CREAT|0666))==-1)
    printf("创建失败\n");
    //信号量初始化
     /*if(INIT_SEM_FULL(sem_id,FULL)==-1)
        printf("初始化信号量失败\n");*/
        if(INIT_SEM_EMPTY(sem_id,EMPTY,ring_buf_num)==-1)
        printf("初始化信号量失败\n");
         if(INIT_SEM_SHM(sem_id,SHM)==-1)
        printf("初始化信号量失败\n");
//创建进程
    pid_t read_id,write_id;
    int wait_tmp;//存放进程终止状态字
        switch(read_id=fork())
        {
            case -1:
            printf("创建进程失败\n");
            case 0:
            readbuf(read_fd,shm_head,sem_id);
            exit(0);
            default://回到父进程
            switch(write_id=fork())
            {
                 case -1:
                printf("创建进程失败\n");
                case 0:
                writebuf(write_fd,shm_head,sem_id);
                exit(0);
                default:
                waitpid(read_id,&wait_tmp,0);
                waitpid(write_id,&wait_tmp,0);
                if(semctl(sem_id,IPC_RMID,0)==-1)
                printf("删除失败\n");
            }
        }
    return 0;
}
int readbuf(int read_fd,int shmid_tail,int semid)
{
    int read_temp;
    ring_buf *ring_buf_tail;//从缓冲区的尾部开始读
    if((ring_buf_tail=(ring_buf*)shmat(shmid_tail,0,0))==(void*)-1)
    printf("readbuf函数中连接共享内存失败\n");
    while(1)
    {
        P(semid,EMPTY);
       // P(semid,SHM);
        read_temp = read(read_fd,ring_buf_tail->data,1024);
        printf("read %d \n",read_temp);
        if(read_temp==-1)
        printf("read操作失败\n");
        else if(read_temp==0)//到达文件尾或者无可读取的数据
        {
            ring_buf_tail->end=1;//记为read的后一个缓冲区
            ring_buf_tail->size=read_temp;
            close(read_fd);
            V(semid,SHM);
            //V(semid,FULL);
            return 0;
        }
        ring_buf_tail->size=read_temp;
        shmid_tail=ring_buf_tail->next_shm;
        if((ring_buf_tail=(ring_buf*)shmat(shmid_tail,0,0))==(void*)-1)
        printf("readbuf函数中连接共享失败\n");
        V(semid,SHM);
        //V(semid,FULL);
    }
 return 0;
}
int writebuf(int write_fd,int shmid_head,int semid)
{
    ring_buf *ring_buf_head;
    if((ring_buf_head=(ring_buf*)shmat(shmid_head,0,0))==(void*)-1)
    printf("writebuf函数中连接共享内存失败\n");
    while(1)
    {
       // P(semid,FULL);
        P(semid,SHM);
        if(ring_buf_head->end==1)
        {
           write(write_fd,ring_buf_head->data,ring_buf_head->size);
            close(write_fd);
         //   V(semid,SHM);
            V(semid,EMPTY);
            return 0;
        }
        int a=write(write_fd,ring_buf_head->data,ring_buf_head->size);
        printf("write %d \n",a);
        shmid_head=ring_buf_head->next_shm;
        if((ring_buf_head=(ring_buf*)shmat(shmid_head,0,0))==(void*)-1)
        printf("write错误\n");
      //  V(semid,SHM);
        V(semid,EMPTY);
    }
    return 0;
}

