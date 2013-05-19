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

void player_load(gchar *player)
{
	FILE *pconf;
	pconf = fopen("player.conf","a+");
	fgets(player,80,pconf);
	fclose(pconf);
}

void player_save(gchar *player)
{
	FILE *pconf;
	pconf = fopen("player.conf","w+");	
	fputs(player,pconf);
	fclose(pconf);
	g_print("save player.conf complete\n");
	}

void init_game_linux(gchar *player)
{
	char com[200] = "java -Xms512m -Xmx1g -cp ";
	strcat(com,"jinput.jar:lwjgl.jar:lwjgl_util.jar:minecraft.jar ");
	strcat(com,"-Djava.library.path=\"./natives\" ");
	strcat(com,"net.minecraft.client.Minecraft ");
	strcat(com,player);
	
	g_print("%s\n",com);
	
	execlp ("bash", "bash", "-c", com, NULL);
	}
	
void init_game_ms(char *player)
{

	}
	
	
void gsign_open_home(GtkWidget *button,gpointer userdata)
{
	execlp("firefox","firefox","http://mc.yiraft.tk",NULL);            //用于linux
	execlp("iexplore","iexplore","http://mc.yiraft.tk",NULL);        //用于windows
	}


void gsign_start_game(GtkWidget *button,gpointer player_buffer)
{	
	gchar player[80] = "";
	strcat(player,gtk_entry_buffer_get_text(player_buffer));
	player_save(player);
	
	chdir("./bin");
	g_print("%s\n",getcwd(NULL,0));
	
	//init_game_linux(player);                           //用于linux
	init_game_ms(player);                                 //用于windows
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

int find_game_linux(GtkWidget *window)
{
	char path[200]="";
	strcat(path,getenv("HOME"));
	strcat(path,"/.minecraft");
	if(chdir(path) != 0 )
	{
		win_erro(window);
		return -1;
			}
	else
			g_print("%s\n",getcwd(NULL,0));
	
	return 0;
	}

int find_game_ms(GtkWidget *window)
{
	if(chdir("./.minecraft") != 0 )
		{	win_erro(window);
			return -1;
		}
	else
		g_print("%s\n",getcwd(NULL,0));
	
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
	
	player_load(player);
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),"Yiwan's craft Launcher");
	gtk_window_set_default_size(GTK_WINDOW(window),320,0);
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window),10);
	g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL);
	
	vbox1 = gtk_vbox_new(FALSE,2);
	gtk_container_add(GTK_CONTAINER(window),vbox1);
	
	image = gtk_image_new_from_file("mclauncher.png");
	gtk_box_pack_start(GTK_BOX(vbox1),image,FALSE,FALSE,0);
	
	vbox2 = gtk_vbox_new(FALSE,3);
	gtk_box_pack_start(GTK_BOX(vbox1),vbox2,TRUE,TRUE,0);
	
	label1 = gtk_label_new("==============输入你的玩家名==============");
	gtk_box_pack_start(GTK_BOX(vbox2),label1,FALSE,FALSE,10);
	
	hbox1 = gtk_hbox_new(FALSE,1);
	gtk_box_pack_start(GTK_BOX(vbox2),hbox1,FALSE,FALSE,0);	
		
	g_print("%s\n",player);
	
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
	
int main(int argc, char **argv)
{		
	GtkWidget *window;
	
	gtk_init(&argc,&argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
	//if(find_game_linux(window) != 0 )                          //用于linux
	if(find_game_ms(window) != 0 )                                //用于windows
	return 0;

	win_main(window);
	gtk_main();
	
	return 0;
	}
