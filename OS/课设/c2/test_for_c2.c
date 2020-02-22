#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
int main()
{
char source[20];
char destination[20];
printf("please input the source file name:");
scanf("%s",source);
printf("please input the destination file name:");
scanf("%s",destination);
int a=syscall(333,source,destination);
printf("%d\n",a);
return 0;
}
