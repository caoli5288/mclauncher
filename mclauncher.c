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

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void conf_player_load(gchar *player)
{
	FILE *pconf;
	pconf = fopen(".minecraft/player.conf","a+");
	fgets(player,80,pconf);
	fclose(pconf);
}

void conf_player_save(gchar *player)
{
	FILE *pconf;
	pconf = fopen(".minecraft/player.conf","w+");	
	fputs(player,pconf);
	fclose(pconf);
	g_print("save player.conf complete\n");
	}

void init_game(gchar *player)
{
	char com[200] = "java -Xms512m -Xmx1g -cp ";
	strcat(com,"jinput.jar:lwjgl.jar:lwjgl_util.jar:minecraft.jar ");
	strcat(com,"-Djava.library.path=\"./natives\" ");
	strcat(com,"net.minecraft.client.Minecraft ");
	strcat(com,player);
	
	g_print("init command:%s\n",com);
	
	execlp ("bash", "bash", "-c", com, NULL);
	}
	
void gsign_open_home(GtkWidget *button,gpointer userdata)
{
	g_print("open home page\n");
	execlp("firefox","firefox","http://mc.yiraft.tk",NULL);
	}


void gsign_start_game(GtkWidget *button,gpointer player_buffer)
{	
	gchar player[80] = "";
	
	strcat(player,gtk_entry_buffer_get_text(player_buffer));
	conf_player_save(player);
	
	chdir(".minecraft/bin");
	g_print("dir now:%s\n",getcwd(NULL,0));
	
	init_game(player); 
	}

void win_erro(GtkWidget *window)
{
	GtkWidget *dialog;	
	dialog = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
	"当前用户家目录下没有.minecraft文件夹\n\n如果您还没有下载游戏,可以到这里下载\n\nhttp://date.yiraft.tk/");
	gtk_window_set_title(GTK_WINDOW(dialog), "找不到游戏");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	}

int find_game(gchar *gamepath)
{
	gchar gamefile[200] = "" ;
	
	g_print("now check game file\n");
	
	strcat(gamefile,gamepath);
	strcat(gamefile,"/.minecraft/bin/minecraft.jar");
	
	if(access(gamefile,F_OK))
		{
			return -1;
			g_print("check game file failed\n");
			}
	g_print("check game file succes\n");
	return 0;
	}

void win_main(GtkWidget *window)
{
	GtkWidget *vbox1,*vbox2,*hbox1,*hbox2;
	GtkWidget *label1;
	GtkWidget *image;
	GtkWidget *entryplayer;
	GtkWidget *button1,*button2;
	GtkEntryBuffer *player_buffer;
	gchar player[80] = "";
	
	g_print("dir when init win_main:%s\n",getcwd(NULL,0));
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),"Yiwan's craft Launcher");
	gtk_window_set_default_size(GTK_WINDOW(window),320,0);
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window),10);
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
	
	conf_player_load(player);									//读取玩家设置
		
	g_print("load player:%s\n",player);
	
	player_buffer = gtk_entry_buffer_new(player,-1);
	
	entryplayer = gtk_entry_new_with_buffer(GTK_ENTRY_BUFFER(player_buffer));
	gtk_entry_set_alignment (GTK_ENTRY (entryplayer), 0.5);
	gtk_box_pack_start(GTK_BOX(hbox1),entryplayer,TRUE,TRUE,0);
		
	hbox2 = gtk_hbox_new(TRUE,2);
	gtk_box_pack_end(GTK_BOX(vbox2),hbox2,FALSE,FALSE,10);
	
	button1 = gtk_button_new_with_label("打开主页");
	button2 = gtk_button_new_with_label("启动游戏");
	
	g_signal_connect(G_OBJECT(button1),"clicked",G_CALLBACK(gsign_open_home),NULL);
	g_signal_connect(G_OBJECT(button2),"clicked",G_CALLBACK(gsign_start_game),player_buffer);
	
	gtk_box_pack_start(GTK_BOX(hbox2),button1,FALSE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(hbox2),button2,FALSE,TRUE,0);
	
	gtk_widget_show_all(window);	
	}
	
void conf_env_set(gchar *gamepath)
{
	gchar env[200] = "HOME = " ;
	
	g_print("now set HOME\n");
	
	strcat(env,gamepath);
	if(putenv(env))
		g_print("set HOME failed\n");
	else
		g_print("set HOME succes\n");
	g_print("now HOME is:%s\n",getenv("HOME"));
	
	chdir(getenv("HOME"));
	}
	
int main(int argc, char **argv)
{	
	gtk_init(&argc,&argv);
	
	GtkWidget *window;
	gchar gamepath[200] = "";
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
	//XXXXXXXXX												//从配置中读取默认目录
	strcat(gamepath,getenv("HOME"));			//读配置做好后注释掉
	
	//if(find_game(window,gamepath))				//从目录中查找游戏文件
	//win_def_dir_set(window);							//配置读写完成后启用，循环检查。

	if(find_game(gamepath))				//从目录中查找游戏文件
	{
		win_erro(window);
		return 0;
		}
	
	conf_env_set(gamepath);							//配置游戏所需环境变量
	
	win_main(window);
	gtk_main();
	
	return 0;
	}
