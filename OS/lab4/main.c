#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>//提供服务性的api接口
#include<sys/stat.h>//描述文件属性
#include<sys/types.h>//包含基本数据类型
#include<dirent.h>//包含opendir函数等
#include<pwd.h>//获取用户名
#include<grp.h>//获取用户组名
#include<time.h>//获取时间
//显示文件的属性
void show_file_status(char* file)
{
    char mode[11];
    strcpy(mode, "----------");
    struct stat stat_buf;
    struct passwd *pwd;
    struct group *group;
    struct tm *time;
    if(stat(file,&stat_buf)!=0)
    printf("stat函数执行失败\n");
//文件类型
	if(S_ISREG(stat_buf.st_mode))
		mode[0]='-';//表示普通文件
    if(S_ISDIR(stat_buf.st_mode))
        mode[0]='d';//表示目录文件
    if(S_ISCHR(stat_buf.st_mode))
    	mode[0]='c';//表示字符设备文件
    if(S_ISBLK(stat_buf.st_mode))
    	mode[0]='b';//表示块设备文件
    if(S_ISFIFO(stat_buf.st_mode))
    	mode[0]='p';//表示管道文件
    if(S_ISLNK(stat_buf.st_mode))
    	mode[0]='l';//表示链接文件
    else
    mode[0]='-';
    //判断USR文件所有者权限
    if(stat_buf.st_mode&S_IRUSR)
    	mode[1]='r';//可读
    if(stat_buf.st_mode&S_IWUSR)
    	mode[2]='w';//可写
    if(stat_buf.st_mode&S_IXUSR)
    	mode[3]='x';//可执行
    //判断GRP用户组权限
    if(stat_buf.st_mode&S_IRGRP)
    	mode[4]='r';//可读
    if(stat_buf.st_mode&S_IWGRP)
    	mode[5]='w';//可写
    if(stat_buf.st_mode&S_IXGRP)
    	mode[6]='x';//可执行
    //判断OTH其他用户权限
    if(stat_buf.st_mode&S_IROTH)
    	mode[7]='r';//可读
    if(stat_buf.st_mode&S_IWOTH)
    	mode[8]='w';//可写
    if(stat_buf.st_mode&S_IXOTH)
    	mode[9]='x';//可执行
//根据用户id获得用户名称
   if((pwd=getpwuid(stat_buf.st_uid))==NULL)
   		printf("getpwuid执行失败\n");
//根据组id获取组名
   if((group=getgrgid(stat_buf.st_gid))==NULL)
   		printf("getgrgid执行失败\n");
   	time=localtime(&stat_buf.st_mtime);
    printf("%s ",mode);//打印文件类型和文件权限
    printf("%d ",(int)stat_buf.st_nlink);//打印硬链接数
    printf("%s ",pwd->pw_name);
    printf("%s ",group->gr_name);
    printf("%ld ",stat_buf.st_size);
    printf("%2d月 %2d %02d:%02d ",time->tm_mon+1,time->tm_mday,time->tm_hour,time->tm_min);
    printf("%s ",file);
    printf("\n");
}
void printdir(char *dir ,int depth)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if ((dp = opendir(dir)) == NULL)
        printf("opendir执行失败\n");
    chdir(dir);
	while ((entry = readdir(dp)) != NULL) {//判断是否读到目录尾
		lstat(entry->d_name, &statbuf);
		if (S_ISDIR(statbuf.st_mode))
		{
            if (strncmp(entry->d_name,  ".", 1) == 0)//把当前目录.，上一级目录..及隐藏文件都去掉，避免死循环遍历目录
				continue;
            printf("%*s%s\n",depth,"",entry->d_name);
            show_file_status(entry->d_name);
			printdir(entry->d_name, depth+4);
		}
		else
            printf("%*s%s\n",depth,"",entry->d_name);
           show_file_status(entry->d_name);

	}
	chdir("..");//回到上一层
	closedir(dp);
}

int main(int argc,char **argv)
{
    char *path = ".";
    struct stat statbuf;
	if (argc >= 2)
    path= argv[1];

	printf("%s\n",path);
	stat(path,&statbuf);
	if(S_ISDIR(statbuf.st_mode))
	printdir(path, 0);

	printf("结束\n");
    return 0;
}
