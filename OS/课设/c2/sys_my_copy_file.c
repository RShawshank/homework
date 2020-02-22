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
