/**
自定义系统调用，实现文件复制。添加在kernel/sys.c文件中
*/
/**
第二步：添加声明：在arch/x86/include/asm/syscalls.h中插入asmlinkage int sys_my_copy_file(char* ,char* );
第三步：添加系统调用id：在arch/x86/entry/syscalls/syscalls_64.tbl中添加 333 64 my_copy_file sys_my_copy_file
第四步：进行内核编译
https://blog.csdn.net/qq_41175905/article/details/80529245
*/
asmlinkage int sys_my_copy_file(char* sourcefile,char* destinationfile)
{
int source = sys_open(sourcefile,O_RDONLY,0);
int destination=sys_open(destinationfile,O_WRONLY|O_CREAT|O_TRUNC,0600);
int i;
char buf[1024];//当buf[]长度小于1024会产生报错：段错误？
//在sys_read和sys_write函数中，其参数buf指向的用户空间的内存地址，
//如果我们直接使用内核空间的指针，则会返回-EFALUT。
//这是因为使用的缓冲区超过了用户空间的地址范围。一般系统调用会要求你使用的缓冲区不能在内核区。
//这个可以用set_fs()、get_fs()来解决。
/*
系统调用本来是提供给用户空间的程序访问的，所以，对传递给它的参数（比如上面的buf），它默认会认为来自用户空间，
在read或write()函数中，为了保护内核空间，一般会用get_fs()得到的值来和USER_DS进行比较，从而防止用户空间程序“蓄意”破坏内核空间；
而现在要在内核空间使用系统调用，此时传递给read或write（）的参数地址就是内核空间的地址了，在USER_DS之上(USER_DS ~ KERNEL_DS)，
如果不做任何其它处理，在write()函数中，会认为该地址超过了USER_DS范围，所以会认为是用户空间的“蓄意破坏”，从而不允许进一步的执行；
为了解决这个问题； set_fs(KERNEL_DS);将其能访问的空间限制扩大到KERNEL_DS,这样就可以在内核顺利使用系统调用了！
*/
mm_segment_t fs;
fs=get_fs();
set_fs(get_ds());
if(source==-1||destination==-1)
{
	printk("file open fail");//函数体中不能有\n
}
if(source>0&&destination>0)
{
	do
	{
	i=sys_read(source,buf,1024);
	sys_write(destination,buf,i);
	}while(i);
printk("copy success!");
}
else
{
	printk("error!");
}
sys_close(source);
sys_close(destination);
set_fs(fs);
return 12;
}
