#include <stdlib.h>
#include<stdio.h>
#include<sys/sysinfo.h>
#include<dirent.h>
#include<time.h>
#include<signal.h>
#include<fcntl.h>
#include<string.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
/*
需要呈现的有
(1)获取并显示主机名
(2)获取并显示系统启动的时间
(3)显示系统到目前为止持续运行的时间
(4)显示系统的版本号
(5)显示cpu的型号和主频大小
(6)同过pid或者进程名查询一个进程，并显示该进程的详细信息，提供杀掉该进程的功能。
(7)显示系统所有进程的一些信息，包括pid，ppid，占用内存大小，优先级等等
(8)cpu使用率的图形化显示(2分钟内的历史纪录曲线)
(9)内存和交换分区(swap)使用率的图形化显示(2分钟内的历史纪录曲线)
(10)在状态栏显示当前时间
(11)在状态栏显示当前cpu使用率
(12)在状态栏显示当前内存使用情况
(13)用新进程运行一个其他程序
(14)关机功能
*/

gboolean draw_cpu_graph();
gboolean cpu_stat(gpointer data);
gboolean cpu_configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer data);
gboolean cpu_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data);
gboolean draw_mem_graph();
gboolean mem_stat(gpointer data);
gboolean mem_configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer data);
gboolean mem_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data);
gboolean now_time_info(gpointer data);
void get_name_info(gchar *name);
void get_cpu_info(gchar *cpu_info);
void get_os_info(gchar *os);
void set_shutdown();
void course_info(GtkListStore *list);
void refresh_pid(gpointer data);
void delete_pid(gpointer data);
gboolean get_uptime_info(gpointer data);
gboolean get_start_time_info(gpointer data);
GtkWidget *main_window;//主窗口
GtkWidget *main_box;//主布局
GtkWidget *notebook;//笔记本控件指针
GtkWidget *resource_label;
GtkWidget *resource_box;
GtkWidget *resource_cpu_frame;
GtkWidget *resource_mem_frame;
GtkWidget *cpu_draw;//绘制cpu使用率的区域
GtkWidget *mem_draw;
GtkWidget *cpu_rate_label;
GtkWidget *mem_rate_label;
GtkWidget *system_label;
GdkPixmap *cpu_graph;//GDK是gtk下面的一层东西，pixmap表示的是内存中的图片缓冲区，将该数据复制到显存中，可以快速更新屏幕
GdkPixmap *mem_graph;
int cpu_time=0,cpu_time0=0,idle0=0,idle=0;
double mem_usage;
double cpu_usage;
gchar cpu_uage[100];
gchar mem_uage[100];
gchar now_time[100];
gint cpu_point[120];
gint mem_point[120];

GtkWidget *sys_label;
GtkWidget *sys_box;
GtkWidget *sys_cpu_frame;
GtkWidget *sys_cpu_label;
GtkWidget *sys_os_frame;
GtkWidget *sys_os_label;
GtkWidget *sys_os_box;
GtkWidget *sys_name_label;
GtkWidget *sys_uptime_label;
GtkWidget *sys_start_time_label;
gchar cpu_info[128]={0};
gchar os_info[128]={0};
gchar name[48];

GtkWidget *exp_label;
GtkWidget *exp_box;
GtkWidget *exp_button_box;
GtkWidget *shutdown_button; //关机按扭
GtkWidget *exit_button; //关闭按钮

GtkWidget *course_label;
GtkWidget *course_box;
GtkWidget *course_box2;
GtkWidget *scroll;//下拉框
GtkWidget *treeView;//列表框
GtkWidget *delete_button;
GtkWidget *refresh_button;
GtkWidget *status_bar;
GtkWidget *status_bar_label;
GtkListStore *list;
GtkCellRenderer* renderer;
GtkTreeViewColumn* column;
gchar *course_info_name[6]={"PID","名称","状态","优先级","占用内存","PPID"};

int main (int argc, char *argv[])
{
    gtk_init(&argc,&argv);//初始化
    //设置主窗口
    main_window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window),"系统监视器");
    gtk_window_set_default_size(GTK_WINDOW(main_window),600,500);
    gtk_window_set_position(GTK_WINDOW(main_window),GTK_WIN_POS_CENTER_ALWAYS);

    //设置主布局
    main_box=gtk_vbox_new(FALSE,5);
    gtk_container_add(GTK_CONTAINER(main_window),main_box);

    //notebook
    notebook=gtk_notebook_new();
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook),GTK_POS_TOP);
    gtk_container_add(GTK_CONTAINER(main_box),notebook);

    //notebook--内存资源
    resource_label=gtk_label_new("内存资源");
    resource_box=gtk_vbox_new(FALSE,5);
    resource_cpu_frame=gtk_frame_new("cpu使用率：");//frame是装饰性的框架
    gtk_box_pack_start(GTK_BOX(resource_box),resource_cpu_frame,FALSE,FALSE,0);//放置在中间
    cpu_draw=gtk_drawing_area_new();
    gtk_drawing_area_size(GTK_DRAWING_AREA(cpu_draw),500,200);
    gtk_container_add(GTK_CONTAINER(resource_cpu_frame),cpu_draw);
    g_timeout_add(1000,draw_cpu_graph,NULL);//间隔1秒刷新一次draw_cpu_graph
    draw_cpu_graph();
    g_signal_connect(cpu_draw, "expose_event", G_CALLBACK(cpu_expose_event), NULL);
    g_signal_connect(cpu_draw, "configure_event", G_CALLBACK(cpu_configure_event), NULL);

    resource_mem_frame = gtk_frame_new("内存使用率:");
    gtk_box_pack_start(GTK_BOX(resource_box), resource_mem_frame, FALSE, FALSE, 0);
    mem_draw = gtk_drawing_area_new();
    gtk_drawing_area_size(GTK_DRAWING_AREA(mem_draw),500,200);
    gtk_container_add(GTK_CONTAINER(resource_mem_frame),mem_draw);
    g_timeout_add(1000, draw_mem_graph, NULL);
    draw_mem_graph();
    g_signal_connect(mem_draw, "expose_event", G_CALLBACK(mem_expose_event), NULL);
    g_signal_connect(mem_draw, "configure_event", G_CALLBACK(mem_configure_event), NULL);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), resource_box, resource_label);

   //notebook----系统信息
    sys_label = gtk_label_new("系统信息");
    sys_box = gtk_vbox_new(FALSE, 5);
    sys_cpu_frame = gtk_frame_new("处理器信息:");
    gtk_box_pack_start(GTK_BOX(sys_box), sys_cpu_frame, TRUE, TRUE, 0);
    sys_os_frame = gtk_frame_new("操作系统信息:");
    gtk_box_pack_start(GTK_BOX(sys_box), sys_os_frame, TRUE, TRUE, 0);

    get_cpu_info(cpu_info);
    sys_cpu_label = gtk_label_new(cpu_info);
    gtk_container_add(GTK_CONTAINER(sys_cpu_frame), sys_cpu_label);

    sys_os_box = gtk_vbox_new(TRUE, 0);
    gtk_container_add(GTK_CONTAINER(sys_os_frame), sys_os_box);

    get_os_info(os_info);
    sys_os_label = gtk_label_new(os_info);
    gtk_container_add(GTK_CONTAINER(sys_os_box), sys_os_label);

    get_name_info(name);
    sys_name_label = gtk_label_new(name);
    gtk_container_add(GTK_CONTAINER(sys_os_box), sys_name_label);


    g_timeout_add(1000, get_uptime_info, NULL);
    get_uptime_info(NULL);
    sys_uptime_label = gtk_label_new(" ");
    gtk_container_add(GTK_CONTAINER(sys_os_box), sys_uptime_label);

    g_timeout_add(1000, get_start_time_info, NULL);
    get_start_time_info(NULL);
    sys_start_time_label = gtk_label_new(" ");
    gtk_container_add(GTK_CONTAINER(sys_os_box), sys_start_time_label);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), sys_box, sys_label);

    //notebook---说明
    exp_label = gtk_label_new("说明");
    exp_box = gtk_vbox_new(FALSE, 5);

    exp_button_box = gtk_hbox_new(TRUE, 10);
    gtk_container_add(GTK_CONTAINER(exp_box), exp_button_box);

    exit_button = gtk_button_new_with_label("关闭任务管理器");
    g_signal_connect(exit_button, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_set_size_request(exit_button, 120, 5);
    gtk_box_pack_start(GTK_BOX(exp_button_box), exit_button, FALSE, FALSE, 0);

    shutdown_button = gtk_button_new_with_label("关闭计算机");
    g_signal_connect(shutdown_button, "clicked", G_CALLBACK(set_shutdown), NULL);
    gtk_widget_set_size_request(shutdown_button, 120, 5);
    gtk_box_pack_start(GTK_BOX(exp_button_box), shutdown_button, FALSE, FALSE, 0);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), exp_box, exp_label);

    //notebook----进程信息
    course_label = gtk_label_new("进程信息");
    course_box = gtk_vbox_new(FALSE, 5);

    scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(scroll, 100, 100);
    gtk_box_pack_start(GTK_BOX(course_box), scroll, TRUE, TRUE, 0);

    list=gtk_list_store_new(6,
                             G_TYPE_STRING,
                             G_TYPE_STRING,
                             G_TYPE_STRING,
                             G_TYPE_STRING,
                             G_TYPE_STRING,
                             G_TYPE_STRING);
    treeView = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list));
    g_object_unref(G_OBJECT(list));//和liststore构件绑定
    gtk_container_add(GTK_CONTAINER(scroll), treeView);

    for(int i = 0; i < 6; i++)
    {
        renderer = gtk_cell_renderer_text_new();//创建一个文本单元绘制器
        //创建一个视图列表
        column = gtk_tree_view_column_new_with_attributes(course_info_name[i], renderer, "text", i, NULL);
        //附加一列列表
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeView), column);
    }
    course_info(list);

    course_box2 = gtk_hbox_new(FALSE, 50);
    gtk_box_pack_start(GTK_BOX(course_box), course_box2, FALSE, FALSE, 0);

    refresh_button = gtk_button_new_with_label("刷新");
    g_signal_connect(G_OBJECT(refresh_button), "clicked", G_CALLBACK(refresh_pid), NULL);
    gtk_widget_set_size_request(refresh_button,60, 30);
    gtk_box_pack_start(GTK_BOX(course_box2), refresh_button, TRUE, FALSE, 0);

    delete_button = gtk_button_new_with_label("结束");
    g_signal_connect(G_OBJECT(delete_button), "clicked", G_CALLBACK(delete_pid), NULL);
    gtk_widget_set_size_request(delete_button, 60, 30);
    gtk_box_pack_start(GTK_BOX(course_box2), delete_button, TRUE, FALSE, 0);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), course_box, course_label);

//状态栏
    status_bar = gtk_statusbar_new();
    gtk_container_add(GTK_CONTAINER(main_box), status_bar);
    gtk_widget_set_size_request(status_bar, 600, 40);
    gtk_box_pack_start(GTK_BOX(main_box), status_bar, TRUE, FALSE, 0);

    g_timeout_add(1000, cpu_stat, NULL);
    cpu_stat(NULL);
    cpu_rate_label = gtk_label_new(cpu_uage);
    gtk_container_add(GTK_CONTAINER(status_bar), cpu_rate_label);

    g_timeout_add(1000, mem_stat, NULL);
    mem_stat(NULL);
    mem_rate_label = gtk_label_new(mem_uage);
    gtk_container_add(GTK_CONTAINER(status_bar), mem_rate_label);

    g_timeout_add(1000, now_time_info, NULL);
    now_time_info(NULL);
    status_bar_label = gtk_label_new(now_time);
    gtk_container_add(GTK_CONTAINER(status_bar), status_bar_label);

    g_signal_connect(main_window,"destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(main_window);
    gtk_main();
    return 0;
}
//获取进程信息
void course_info(GtkListStore *list)
{
    int fp;
    DIR *open_dir;
    GtkTreeIter itertor;
    gchar pid_path[256];
    gchar content[128];
    gchar *temp1[128];
    gchar *temp;
    gchar *pid;
    gchar *ppid;
    gchar pid_name[1024];
    gchar *pid_state;
    gchar *pid_priority;
    gchar pid_mem[10];
    struct dirent *pid_info;
    open_dir=opendir("/proc");
    while((pid_info=readdir(open_dir))!=NULL)
    {
        //依次获取进程的信息
        if(pid_info->d_name[0]>'0'&&pid_info->d_name[0]<='9')
        {
            sprintf(pid_path,"/proc/%s/stat",pid_info->d_name);
            fp=open(pid_path,O_RDONLY);
            read(fp,content,128);
            close(fp);

            pid=strtok(content," ");

            temp=strtok(NULL,")");
            strncpy(pid_name,temp+1,64);

            for(int i=2;i<23;i++)
            {
              temp1[i]=strtok(NULL," ");
            }
            switch(temp1[2][0])
            {
                case 'R':pid_state="running";break;
                case 'S':pid_state="sleeping";break;
                case 'I':pid_state="idle";break;
                case 'D':pid_state="disk sleep";break;
                case 'T':pid_state="stopped";break;
                case 'Z':pid_state="zombie";break;
                case 'X':pid_state="dead";break;
            }
            ppid=temp1[3];
            pid_priority=temp1[17];
            double mem=atoi(temp1[22])/(1024*1024);
            sprintf(pid_mem,"%-3.f MB",mem);
            gtk_list_store_append(list,&itertor);
            gtk_list_store_set(list,&itertor,
                               0,pid,
                               1,pid_name,
                               2,pid_state,
                               3,pid_priority,
                               4,pid_mem,
                               5,ppid,
                               -1);

        }
    }
    closedir(open_dir);
}
//刷新进程信息
void refresh_pid(gpointer data)
{
    gtk_list_store_clear(list);
    course_info(list);
}

//结束进程
void delete_pid(gpointer data)
{
    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter itertor;
    gchar *pid;
    pid_t pid_int;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeView));
    if(gtk_tree_selection_get_selected(selection, &model, &itertor))
    {
        gtk_tree_model_get(model, &itertor, 1, &pid ,-1);
        pid_int = atoi(pid);
        kill(pid_int, SIGTERM);
        gtk_list_store_clear(list);
        course_info(list);
    }
}
//获取主机名
void get_name_info(gchar *name)
{
    char temp[20];
    gethostname(temp,sizeof(temp));
    sprintf(name,"主机名：\t%s",temp);
}
//获取cpu信息
void get_cpu_info(gchar *cpu_info)
{
    FILE *fp;
    fp=fopen("/proc/cpuinfo","r");
    int i=0;
    gchar temp[1000]={0};
    gchar *str1,*str2;
    if(fp==NULL)
     {
         printf("failed to open cpuinfo\n");
     }
     while(!feof(fp)&&i!=2)
    {
          memset(temp,0,sizeof(temp));
        fgets(temp,sizeof(temp)-1,fp);
        if(str1=strstr(temp,"model name"))
        {
            strcpy(cpu_info,str1);
            i++;
        }
        if(str2=strstr(temp,"cpu MHz"))
        {
            sprintf(cpu_info,"%s%s",cpu_info,str2);
            i++;
        }
    }

    fclose(fp);
}
//获取操作系统信息
void get_os_info(gchar *os_info)
{
    FILE *fp;
    gchar temp[33]={0};
    gchar *str1;
    fp=fopen("/proc/version","r");
    if(fp==NULL)
    {
         printf("failed to open version\n");
    }
    while(!feof(fp))
    {
        memset(temp,0,sizeof(temp));
        fgets(temp,sizeof(temp)-1,fp);
        if(str1=strstr(temp,"version"))
        {
            strcpy(temp,str1);
            break;
        }
    }
    sprintf(os_info,"Linux   :\t%s",temp);
    fclose(fp);
}
//获取系统运行时间
gboolean get_uptime_info(gpointer data)
{
    struct sysinfo info;
    gchar uptime[128];
    if(sysinfo(&info))
    {
        printf("failed to get sysinfo\n");
    }
    long timenum=info.uptime;
    int day = timenum/86400;
    int hour=(timenum%86400)/3600;
    int min =(timenum%3600)/60;
    int sec=(timenum%60);
    bzero(uptime,128);//置0
    sprintf(uptime,"系统已运行了：%d天%d时%d分%d秒",day,hour,min,sec);
    gtk_label_set_text(GTK_LABEL(sys_uptime_label), uptime);
return TRUE;
}
gboolean get_start_time_info(gpointer data)
{
     struct sysinfo info;
     struct tm *tm;
     gchar starttime[128];
     long uptime=0;
     time_t time_now;
     if(sysinfo(&info))
    {
        printf("failed to get sysinfo\n");
    }
    uptime=info.uptime;
    //获得当前时间

    time(&time_now);
    if(time_now>uptime)
    {
        time_now-=uptime;
    }
    else
    {
        time_now=uptime-time_now;
    }
    tm=localtime(&time_now);
    bzero(starttime,128);
    sprintf(starttime,"系统启动时间：%d-%d-%d %d:%d:%d\n",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
    gtk_label_set_text(GTK_LABEL(sys_start_time_label), starttime);
return TRUE;
}


gboolean cpu_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    gdk_draw_drawable(widget->window, widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
                    cpu_graph, event->area.x, event->area.y, event->area.x, event->area.y,
                    event->area.width, event->area.height);

    return TRUE;
}
//获取cpu的利用率
gboolean cpu_stat(gpointer data)
{
    FILE *fp;
    fp = fopen("/proc/stat","r");
    char ch[100];
    char cpu[10];
    fgets(ch,sizeof(ch),fp);
    fclose(fp);
    //用户态时间，漂亮的用户态时间，内核态时间，空闲时间，IO等待时间，硬中断时间，软中断时间，被盗时间（虚拟化环境运行其他操作系统的时间），来宾时间（操作系统运行虚拟cpu花费的时间）
    int user,nice,system,idle,iowait,irq,softirq;
    sscanf(ch,"%s%d%d%d%d%d%d%d",cpu,&user,&nice,&system,&idle,&iowait,&irq,&softirq);
    long cpu_time = user+system+idle+iowait+irq+softirq;
    double cpu_usage = (double)((cpu_time-cpu_time0)-(idle-idle0))*100/(double)(cpu_time-cpu_time0);
    sprintf(cpu_uage,"cpu利用率：%.2f%%",cpu_usage);
    idle0=idle;
    cpu_time0=cpu_time;
    gtk_label_set_text(GTK_LABEL(cpu_rate_label),cpu_uage);
    return TRUE;
}
gboolean cpu_configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{
    if(cpu_graph){
        g_object_unref(cpu_graph);
    }
    cpu_graph = gdk_pixmap_new(widget->window, widget->allocation.width, widget->allocation.height, -1);
    gdk_draw_rectangle(cpu_graph, widget->style->white_gc, TRUE, 0, 0,
                        widget->allocation.width, widget->allocation.height);

    return TRUE;
}
//绘制cpu使用率图形
gboolean draw_cpu_graph()
{
    if(cpu_graph==NULL) return TRUE;
    gint current, distance;
    gint x, y;
    gdk_draw_rectangle(GDK_DRAWABLE(cpu_graph), main_window->style->white_gc, TRUE, 0, 0,
                        cpu_draw->allocation.width, cpu_draw->allocation.height);
    x = cpu_draw->allocation.width;
    y = cpu_draw->allocation.height;

    current = (int)(cpu_usage*(double)y);
    for(int i = 0; i < 119; i++)
    {
        cpu_point[i] = cpu_point[i+1];
    }
    cpu_point[119] = y - current -10;

    distance = x / 120;
    GdkGC *gc = gdk_gc_new(GDK_DRAWABLE(cpu_graph));

    GdkColormap *colormap;
    colormap = gdk_colormap_get_system();
    GdkColor color;
    gdk_color_parse("red", &color);
    gdk_colormap_alloc_color(colormap, &color ,FALSE, FALSE);
    gdk_gc_set_foreground(gc, &color);

    for(int i = 119; i > 0; i--)
    {
        gdk_draw_line(GDK_DRAWABLE(cpu_graph), gc, i*distance, cpu_point[i], (i-1)*distance, cpu_point[i-1]);
    }

    gtk_widget_queue_draw(cpu_draw);
    return TRUE;
}
gboolean mem_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    gdk_draw_drawable(widget->window, widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
                    cpu_graph, event->area.x, event->area.y, event->area.x, event->area.y,
                    event->area.width, event->area.height);

    return TRUE;
}

gboolean mem_stat(gpointer data)
{
    FILE *fp;
    fp = fopen("/proc/meminfo","r");
    char ch[256];
    double mem_total,mem_free;
    fgets(ch,sizeof(ch),fp);
    char *s=strtok(ch," ");
    s=strtok(NULL," ");
    mem_total=atoi(s);
    fgets(ch,sizeof(ch),fp);
    s=strtok(ch," ");
    s=strtok(NULL," ");
    mem_free=atoi(s);
    mem_usage = (double)1-((double)mem_free/(double)mem_total);
    sprintf(mem_uage, "内存剩余：%-2.fMB/%-2.fMB", mem_free/1024, mem_total/1024);
    gtk_label_set_text(GTK_LABEL(mem_rate_label),mem_uage);
    fclose(fp);
return TRUE;
}
gboolean mem_configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{
    if(mem_graph){
        g_object_unref(mem_graph);
    }
    mem_graph = gdk_pixmap_new(widget->window, widget->allocation.width, widget->allocation.height, -1);
    gdk_draw_rectangle(mem_graph, widget->style->white_gc, TRUE, 0, 0,
                        widget->allocation.width, widget->allocation.height);

    return TRUE;
}
//绘制cpu使用率图形
gboolean draw_mem_graph()
{
    if(mem_graph==NULL) return TRUE;
    gint current, distance;
    gint x, y;
    gdk_draw_rectangle(GDK_DRAWABLE(mem_graph), main_window->style->white_gc, TRUE, 0, 0,
                        mem_draw->allocation.width, mem_draw->allocation.height);
    x = mem_draw->allocation.width;
    y = mem_draw->allocation.height;

    current = (int)(mem_usage*(double)y);
    for(int i = 0; i < 119; i++)
    {
        mem_point[i] = mem_point[i+1];
    }
    mem_point[119] = y - current -10;

    distance = x / 120;
    GdkGC *gc = gdk_gc_new(GDK_DRAWABLE(mem_graph));

    GdkColormap *colormap;
    colormap = gdk_colormap_get_system();
    GdkColor color;
    gdk_color_parse("red", &color);
    gdk_colormap_alloc_color(colormap, &color ,FALSE, FALSE);
    gdk_gc_set_foreground(gc, &color);

    for(int i = 119; i > 0; i--)
    {
        gdk_draw_line(GDK_DRAWABLE(mem_graph), gc, i*distance, mem_point[i], (i-1)*distance, mem_point[i-1]);
    }

    gtk_widget_queue_draw(mem_draw);
    return TRUE;
}
//获取当前时间
gboolean now_time_info(gpointer data)
{
    time_t tim;
    struct tm * time_now;

    time(&tim);
    time_now= localtime(&tim);

    sprintf(now_time, "时间：%04d-%02d-%02d %02d:%02d:%02d",
            time_now->tm_year + 1900, time_now->tm_mon + 1, time_now->tm_mday,
            time_now->tm_hour, time_now->tm_min, time_now->tm_sec);

    gtk_label_set_text(GTK_LABEL(status_bar_label), now_time);

    return TRUE;
}
void set_shutdown()
{
    system("shutdown now");
}
