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

void initgamelinux(char *player)
{
	char com[200] = "java -Xms512m -Xmx1g -cp ";
	strcat(com,"jinput.jar:lwjgl.jar:lwjgl_util.jar:minecraft.jar ");
	strcat(com,"-Djava.library.path=\"./natives\" ");
	strcat(com,"net.minecraft.client.Minecraft ");
	strcat(com,player);
	
	g_print("\n%s",com);
	
	execl ("/bin/bash", "bash", "-c", com, NULL);
	}
	
void initgamems(char *player)
{
	execlp("javaw","javaw","-cp","/jinput.jar;lwjgl.jar;lwjgl_util.jar;minecraft.jar","-Djava.library.path=\"./natives\"","net.minecraft.client.Minecraft",player,NULL);
	}
	
void initsetwin()
{
	
	}
	
void initpointdownwin()
{

	}
	
void setconf(GtkWidget *button,gpointer userdata)
{
	initsetwin();
	}

void startgame(GtkWidget *button,gpointer bufferplayer)
{
	FILE *pconf;
	char player[80] = "";
	
	pconf = fopen("player.conf","w+");
	strcat(player,gtk_entry_buffer_get_text(bufferplayer));
	
	fputs(player,pconf);
	fclose(pconf);
	
	g_print("%s\n",player);
	
	chdir("./bin");
	g_print("%s\n",getcwd(NULL,0));
	
	//initgamelinux(player);
	initgamems(player);
	}

void findgamelinux()
{
	char path[200]="";
	strcat(path,getenv("HOME"));
	strcat(path,"/.minecraft");
	if(chdir(path) != 0 )
			initpointdownwin();
	else
			g_print("%s\n",getcwd(NULL,0));
	}

void findgamems()
{
	if(chdir("./.minecraft") != 0 )
		initpointdownwin();
	else
		g_print("%s\n",getcwd(NULL,0));
	}

void initmainwin()
{
	char player[80] = "";
	FILE *pconf;
	GtkWidget *window1;
	GtkWidget *vbox1,*vbox2,*hbox1,*hbox2;
	GtkWidget *image1;
	GtkWidget *label1;
	GtkWidget *entryplayer;
	GtkWidget *button1,*button2;
	GtkEntryBuffer *bufferplayer;
	
	window1 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window1),"Yiwan's craft Launcher");
	gtk_window_set_default_size(GTK_WINDOW(window1),320,400);
	gtk_widget_set_size_request (window1, 320, 400);
	gtk_window_set_resizable(GTK_WINDOW(window1),FALSE);
	gtk_window_set_position(GTK_WINDOW(window1),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window1),10);
	g_signal_connect(G_OBJECT(window1),"delete_event",G_CALLBACK(gtk_main_quit),NULL);
	
	vbox1 = gtk_vbox_new(FALSE,2);
	gtk_container_add(GTK_CONTAINER(window1),vbox1);
	
	image1 = gtk_image_new_from_file("mclauncher.png");
	gtk_box_pack_start(GTK_BOX(vbox1),image1,FALSE,FALSE,0);
	
	vbox2 = gtk_vbox_new(FALSE,3);
	gtk_box_pack_start(GTK_BOX(vbox1),vbox2,TRUE,TRUE,0);
	
	label1 = gtk_label_new("输入你的用户名");
	gtk_box_pack_start(GTK_BOX(vbox2),label1,FALSE,FALSE,10);
	
	hbox1 = gtk_hbox_new(FALSE,1);
	gtk_box_pack_start(GTK_BOX(vbox2),hbox1,FALSE,FALSE,0);	
	
	//findgamelinux();
	findgamems();
	
	pconf = fopen("player.conf","a+");
	fgets(player,80,pconf);
	fclose(pconf);
	
	g_print("%s\n",player);
	
	bufferplayer = gtk_entry_buffer_new(player,-1);
	
	entryplayer = gtk_entry_new_with_buffer(GTK_ENTRY_BUFFER(bufferplayer));
	gtk_box_pack_start(GTK_BOX(hbox1),entryplayer,TRUE,TRUE,60);
		
	hbox2 = gtk_hbox_new(TRUE,2);
	gtk_box_pack_end(GTK_BOX(vbox2),hbox2,FALSE,FALSE,10);
	
	button1 = gtk_button_new_with_label("设置选项");
	button2 = gtk_button_new_with_label("启动游戏");
	
	g_signal_connect(G_OBJECT(button1),"clicked",G_CALLBACK(setconf),NULL);
	g_signal_connect(G_OBJECT(button2),"clicked",G_CALLBACK(startgame),bufferplayer);
	
	gtk_box_pack_start(GTK_BOX(hbox2),button1,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(hbox2),button2,FALSE,FALSE,0);
	
	gtk_widget_show_all(window1);	
	}
	
int main(int argc, char **argv)
{	
	gtk_init(&argc,&argv);
	initmainwin();
	gtk_main();
	
	return 0;
}

