#include <stdlib.h>
#include <gtk/gtk.h>
void destroy()
{
    gtk_main_quit();
}
    GtkWidget *window1 ;
    GtkWidget *button1 ;
    GtkWidget *label1 ;
    GtkWidget *vbox1 ;
    GtkWidget *window2;
    GtkWidget *button2 ;
    GtkWidget *label2 ;
    GtkWidget *vbox2 ;
    GtkWidget *window3;
    GtkWidget *button3 ;
    GtkWidget *label3;
    GtkWidget *vbox3;
int main (int argc, char *argv[])
{
pid_t p1,p2;
switch(p1=fork())
{
    case -1:
    printf("进程创建失败");
    case 0:

    gtk_init (&argc, &argv);//初始化图形显示环境
    window1=gtk_window_new (GTK_WINDOW_TOPLEVEL);//创建窗口
    vbox1=gtk_vbox_new (FALSE, 10);//创建一个垂直盒子
    gtk_container_set_border_width(GTK_CONTAINER(vbox1),100);

    label1=gtk_label_new("child1");
    button1=gtk_button_new_with_label("close the process");
    g_signal_connect(button1,"clicked",G_CALLBACK(gtk_main_quit),window1);//指定按钮监听事件
    gtk_container_add(GTK_CONTAINER(window1),vbox1);//将垂直盒子加入到窗口中
    gtk_box_pack_start(GTK_BOX(vbox1),label1,FALSE,FALSE,10);//标签加入到盒子中
    gtk_box_pack_start(GTK_BOX(vbox1),button1,FALSE,FALSE,10);

    g_signal_connect(window1,"destroy",G_CALLBACK(gtk_main_quit),NULL);

    gtk_widget_show_all(window1);
    gtk_main();//进入到消息处理循环
    default:
    switch(p2=fork())
    {
        case -1:
        printf("进程创建失败");
        case 0:

        gtk_init (&argc, &argv);//初始化图形显示环境
        window2=gtk_window_new (GTK_WINDOW_TOPLEVEL);//创建窗口
        vbox2=gtk_vbox_new (FALSE, 10);//创建一个垂直盒子
        gtk_container_set_border_width(GTK_CONTAINER(vbox2),100);

        label2=gtk_label_new("child2");
        button2=gtk_button_new_with_label("close the process");
        g_signal_connect(button2,"clicked",G_CALLBACK(gtk_main_quit),window2);//指定按钮监听事件
        gtk_container_add(GTK_CONTAINER(window2),vbox2);//将垂直盒子加入到窗口中
        gtk_box_pack_start(GTK_BOX(vbox2),label2,FALSE,FALSE,10);//标签加入到盒子中
        gtk_box_pack_start(GTK_BOX(vbox2),button2,FALSE,FALSE,10);

        g_signal_connect(window2,"destroy",G_CALLBACK(gtk_main_quit),NULL);

        gtk_widget_show_all(window2);
        gtk_main();//进入到消息处理循环
        default:

        gtk_init (&argc, &argv);//初始化图形显示环境
        window3=gtk_window_new (GTK_WINDOW_TOPLEVEL);//创建窗口
        vbox3=gtk_vbox_new (FALSE, 10);//创建一个垂直盒子
        gtk_container_set_border_width(GTK_CONTAINER(vbox3),100);

        label3=gtk_label_new("parent");
        button3=gtk_button_new_with_label("close the process");
        g_signal_connect(button3,"clicked",G_CALLBACK(gtk_main_quit),window3);//指定按钮监听事件
        gtk_container_add(GTK_CONTAINER(window3),vbox3);//将垂直盒子加入到窗口中
        gtk_box_pack_start(GTK_BOX(vbox3),label3,FALSE,FALSE,10);//标签加入到盒子中
        gtk_box_pack_start(GTK_BOX(vbox3),button3,FALSE,FALSE,10);

        g_signal_connect(window3,"destroy",G_CALLBACK(gtk_main_quit),NULL);

        gtk_widget_show_all(window3);
        gtk_main();//进入到消息处理循环
    }

}

  return 0;
}
