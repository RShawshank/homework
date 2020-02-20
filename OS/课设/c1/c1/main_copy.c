#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<dirent.h>//输出文件信息
#include<sys/stat.h>//判断是否是目录
#include<sys/types.h>
#include<fcntl.h>
void copy_file(char* source_name,char* destination_name)
{
    int file_source;
    int file_destination;
    int flag;
    char buf[1024];
    file_source=open(source_name,O_RDWR);
    file_destination=open(destination_name,O_CREAT|O_RDWR,S_IRUSR|S_IWUSR|S_IXUSR);
    if(file_source==-1||file_destination==-1)
    {
        printf("文件打开或者创建失败\n");
        exit(-1);
    }

    while((flag=read(file_source,buf,1024))>0)
    {
        write(file_destination,buf,flag);
    }
    close(file_source);
    close(file_destination);
}
void copy_folder(char* source,char* destination)
{
    if(!opendir(destination))
    {
        if(mkdir(destination,0777))//如果不存在就创建该文件夹
            printf("创建文件夹失败！\n");
    }
    DIR* dp = opendir(source);
    struct dirent* filename;
    while(filename=readdir(dp))//遍历DIR指针指向的文件夹，即文件数组
    {
        int len = strlen(source)+strlen(filename->d_name)+5;//有分割符号
        char *file_source_path = (char*)malloc(sizeof(char)*len);
        char *file_destination_path=(char*)malloc(sizeof(char)*len);
        strcpy(file_source_path,source);
        strcpy(file_destination_path,destination);
        strcat(file_source_path,"/");
        strcat(file_destination_path,"/");
        //取文件名与当前文件名拼接成一个完整的路径
        strcat(file_source_path,filename->d_name);
        strcat(file_destination_path,filename->d_name);
        struct stat statbuf;
        lstat(file_source_path,&statbuf);//获取文件信息
        //如果是目录的话
        if(S_ISDIR(statbuf.st_mode))
        {
            if(strcmp(".",filename->d_name)!=0&&strcmp("..",filename->d_name)!=0)
            {
                copy_folder(file_source_path,file_destination_path);//进入该文件夹复制，第归调用
            }
        }
        else{
            copy_file(file_source_path,file_destination_path);//对文件进行复制
            printf("复制%s到%s成功！\n",file_source_path,file_destination_path);
        }
        free(file_source_path);
        free(file_destination_path);
    }

}
int main (int argc,char *argv[])
{
    char source_name[20];
    char destination_name[20];
    printf("please input the source file name:");
    scanf("%s",source_name);
    printf("please input the destination file name:");
    scanf("%s",destination_name);
    DIR *source=opendir(source_name);//获取子目录下的所有文件和目录列表
    DIR *destination = opendir(destination_name);
    if(!source)
    {
        printf("输入的参数不是文件夹！\n");
        copy_file(source_name,destination_name);
    }
    else
    copy_folder(source_name,destination_name);
    return 0;
}
