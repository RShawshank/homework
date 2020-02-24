#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
int main()
{
    char buf[256];
	char get[256];
	int fp = open("/dev/myDevice",O_RDWR);
	if(fp == -1)
	{
	    printf("open fail!\n");
		return 0;
	}
	else
    {
	read(fp,buf,sizeof(buf));
	printf("%s\n", buf);
	printf("Please input something you want to write:");
	scanf("%s", get);
	//测试写操作
	write(fp,get,sizeof(get));
	//测试读操作
	read(fp,buf,sizeof(buf));
	printf("The thing is %s\n", buf);
	close(fp);
    return 0;
    }
}
