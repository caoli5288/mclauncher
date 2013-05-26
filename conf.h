#include <unistd.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

gchar *conf[4];

void conf_player_load()
{
	FILE *pconf;
	gchar player[40] = "";
	
	pconf = fopen(".minecraft/mclauncher.conf","a+");
	fgets(player,40,pconf);
	fclose(pconf);
	
	conf[1] = player;
	
	g_print("load player:%s\n",conf[1]);
	}

void conf_player_save()
{
	FILE *pconf;
	pconf = fopen(".minecraft/mclauncher.conf","w+");	
	fputs(conf[1],pconf);
	fclose(pconf);
	g_print("save mclauncher.conf complete\n");
	}
	
void conf_envp_set()
{
	gchar envp[80] = "HOME=";
	strcat(envp,conf[0]);
	putenv(envp);
	
	g_print("now HOME at:%s",getenv("HOME"));
	}
	
int conf_game_find()
{
	gchar gamefile[200] = "" ;

	g_print("now check game file\n");
	
	strcat(gamefile,conf[0]);
	strcat(gamefile,"/.minecraft/bin/minecraft.jar");
	
	if(access(gamefile,F_OK))
		{
			return -1;
			g_print("check game file failed\n");
			}
	g_print("check game file succes\n");
	return 0;
	}
