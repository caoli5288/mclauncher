/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */
 
#include "conf.h"

void init_game()
{
	gchar argv[200] = "java ";
	strcat(argv,conf[2]);
	strcat(argv," ");
	strcat(argv,conf[3]);
	strcat(argv," -cp ");
	strcat(argv,"jinput.jar:lwjgl.jar:lwjgl_util.jar:minecraft.jar ");
	strcat(argv,"-Djava.library.path=\"./natives\" ");
	strcat(argv,"net.minecraft.client.Minecraft ");
	strcat(argv,conf[1]);
	
	conf_envp_set();
	chdir(".minecraft/bin");
	
	//system(argv);
	gtk_main_quit();
	}
	
void win_conf()
{
	GtkWidget *frame;
	GtkWidget *window;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),"设置窗口");
	gtk_window_set_default_size(GTK_WINDOW(window),400,150);
	//gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
	//gtk_widget_set_size_request
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window),5);
	
	frame = gtk_frame_new("内存设置");
	gtk_container_add(GTK_CONTAINER(window),GTK_WIDGET(frame));
	
	gtk_widget_show_all(window);
	gtk_main_iteration();
	}

void win_erro()
{
	GtkWidget *dialog;
	GtkWidget *window;
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	dialog = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
	"当前用户家目录下没有.minecraft文件夹\n\n如果您还没有下载游戏,可以到这里下载\n\nhttp://date.yiraft.tk/");
	gtk_window_set_title(GTK_WINDOW(dialog), "找不到游戏");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	}

void gsign_start_game(GtkWidget *button,void *pack[])
{	
	gtk_widget_hide_all((pack[0]));
	g_print((gchar*)pack[2]);
	gchar player[40] = "";
	g_print("检测\n");
	strcat(player,gtk_entry_buffer_get_text((GtkEntryBuffer*)pack[1]));
	g_print("检测\n");
	conf[1] = player;
	conf_player_save();	
	init_game(); 
	}
	
void gsign_conf_set(GtkWidget *button,gpointer userdata)
{
	win_conf();
	}
	
void win_main()
{
	GtkWidget *window;
	GtkWidget *vbox1,*vbox2,*hbox1,*hbox2;
	GtkWidget *label1;
	GtkWidget *image;
	GtkWidget *entryplayer;
	GtkWidget *button1,*button2;
	GtkEntryBuffer *player_buffer;
	
	gchar player[40] = "";
	strcat(player,conf[1]);
	
	g_print("dir when init win_main:%s\n",getcwd(NULL,0));
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),"Yiwan's craft Launcher");
	gtk_window_set_default_size(GTK_WINDOW(window),320,0);
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window),5);
	g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL);
	
	vbox1 = gtk_vbox_new(FALSE,2);
	gtk_container_add(GTK_CONTAINER(window),vbox1);
	
	image = gtk_image_new_from_file(".minecraft/mclauncher.png");
	gtk_box_pack_start(GTK_BOX(vbox1),image,FALSE,FALSE,0);
	
	vbox2 = gtk_vbox_new(FALSE,3);
	gtk_box_pack_start(GTK_BOX(vbox1),vbox2,TRUE,TRUE,0);
	
	label1 = gtk_label_new("==============输入你的玩家名==============");
	gtk_box_pack_start(GTK_BOX(vbox2),label1,FALSE,FALSE,10);
	
	hbox1 = gtk_hbox_new(FALSE,1);
	gtk_box_pack_start(GTK_BOX(vbox2),hbox1,FALSE,FALSE,0);	
	
	player_buffer = gtk_entry_buffer_new(player,-1);
	
	entryplayer = gtk_entry_new_with_buffer(player_buffer);
	gtk_entry_set_alignment (GTK_ENTRY (entryplayer), 0.5);
	gtk_box_pack_start(GTK_BOX(hbox1),entryplayer,TRUE,TRUE,0);
		
	hbox2 = gtk_hbox_new(TRUE,2);
	gtk_box_pack_end(GTK_BOX(vbox2),hbox2,FALSE,FALSE,5);
	
	button1 = gtk_button_new_with_label("选项设置");
	button2 = gtk_button_new_with_label("启动游戏");
	
	gchar test[] = "测试\n";
	g_print(test);
	void *pack[] = {window,player_buffer,test};
	g_print(pack[2]);
	
	g_signal_connect(G_OBJECT(button1),"clicked",G_CALLBACK(gsign_conf_set),NULL);
	g_signal_connect(G_OBJECT(button2),"clicked",G_CALLBACK(gsign_start_game),pack);
	
	gtk_box_pack_start(GTK_BOX(hbox2),button1,FALSE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(hbox2),button2,FALSE,TRUE,0);
	
	gtk_widget_show_all(window);
	}
	
int main(int argc, char **argv)
{	
	conf[0] = getenv("HOME");
	conf[1] = "";
	conf[2] = "-Xms512m";
	conf[3] = "-Xmx1024m";
	
	gtk_init(&argc,&argv);
	
	if(conf_game_find())
	{
		win_erro();
		return 0;
		}
	
	chdir(conf[0]);
	conf_player_load();
	
	win_main();
	gtk_main();
	
	return 0;
	}
