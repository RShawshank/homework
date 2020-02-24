#include <linux/module.h>//包含了许多符号与函数定义，多与加载模块有关
#include <linux/kernel.h>
#include <linux/init.h>//包含了模块初始化与清除的函数
#include <linux/uaccess.h>
#include<linux/fs.h>//包含三个字符设备函数

static char* devName = "myDevice";//设备名
static int device_number=0;//设备号
static char message[256]="OS_my_device";
//linux字符设备驱动


//inode为文件节点（无论用户打开多少个文件，都只有一个inode结构对应）
//计数的原因是因为模块自身在被使用时，是不允许被卸载的
static int my_open(struct inode *inode,struct file *file)
{
    try_module_get(THIS_MODULE);//判断该模块是否处于活动状态，通过local_inc()宏将该模块的引用+！
	return 0;
}
static int my_release(struct inode *inode,struct file *file)
{
    module_put(THIS_MODULE);
	return 0;
}
//参数：file为进行读取信息的目标文件，buffer为对应防治信息的缓冲区（用户空间内存地址）
//size读取信息的长度,p为读的位置相对于文件开头的偏移
static ssize_t my_read(struct file *file,char __user *buffer,size_t size, loff_t *p)
{
    if(copy_to_user(buffer,message,size))
    {
        return -EFAULT;
    }
    return sizeof(message);
}
//ppos当前的偏移位置，通常用来判断写文件是否越界
static ssize_t my_write(struct file *file,const char __user *buffer,size_t size, loff_t *ppos)
{
    if(copy_from_user(message,buffer,size))
    {
        return -EFAULT;
    }
    return sizeof(message);
}
static struct file_operations my_dev=
{
    .read=my_read,//异步的读取操作
    .open=my_open,//打开
    .write=my_write,//异步的写入操作
    .release=my_release,//关闭
    .owner=THIS_MODULE,//拥有该结构的指针
};
//注册模块
static int init_module_func(void)
{
    //module_init()调用该函数进行字符设备的注册
    //参数1：新注册的设备的主设备由系统分配，设备号为0时表示系统会动态分配一个设备号创建模块
    //参数2：新设备注册时的设备名字
    //参数3：指向file_operations指针
    int ret=register_chrdev(0,devName,&my_dev);
    if(ret<0)
    {
        printk("register fail!");
        return -1;
    }
    else
    {
        device_number=ret;
        printk("register successfully!");
    }
    return device_number;

}
static void cleanup_module_func(void)
{
    unregister_chrdev(device_number,devName);
    printk("unregister successfully!");
}

module_init(init_module_func);//在插入内核的时候启动，在内核中注册一定的功能函数
module_exit(cleanup_module_func);//当内核模块卸载时，调用它将模块从内核中清除
MODULE_LICENSE("GPL");//模块许可证，必须写在函数之外！
