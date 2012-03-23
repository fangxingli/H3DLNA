#ifndef _CRENDERER_H
#define _CRENDERER_H

#include <iostream>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/wait.h>
 #include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/wait.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

//#include <clutter/x11/clutter-x11.h>
//#include <X11/Xlib.h>

//#include <clutter/clutter.h>
//#include <mx/mx.h>

#include <errno.h>
#include <stdlib.h>
//#include <glib.h>
//#include <gmodule.h>

//#include "intfUtil.h"

#define DEBUG

#define DESC_URL_SIZE 200
#define	DUP2CLOSE(oldfd, newfd)	(dup2(oldfd, newfd) == 0  &&  close(oldfd) == 0)

#define	DATABUFSIZE	4096	/* the length of the buffer read in "fd" */
#define CMDBUFSIZE 4096

using namespace std;


enum
{
	STOP,
	PLAY,
	PAUSE
};

enum
{
	ADDTOPLAYLIST,
	PLAYRIGHTNOW
	
};

enum
{
	MPSTARTFAILED = -1,
	MPSTARTOK
};


class cRenderer {

private:
	// thread

	int iret;
	#define	PARENT_READ	readpipe[0]
	#define	CHILD_WRITE	readpipe[1]
	#define CHILD_READ	writepipe[0]
	#define PARENT_WRITE	writepipe[1]

public:
	FILE* readfp;	
	

	//pipe file
	int	writepipe[2],	/* parent -> child */
		readpipe [2],	/* child -> parent */
		errpipe[2];
	pid_t childpid;
	
	
private:
	char *mplayer_bin;
    	char* fullscreen;
    	pthread_t thread;
    	pthread_t thread1;
//GMainLoop * loop;

	bool mplayer_is_running;
	int positionToSeek;
	int speed;
	int playstatus;
	int playvolume;
	int playmute;
	float playduration;
	float playposition;

	int argc;
	char **argv;
//	ClutterActor * stage;
//	X_TIMER *timer;
	
	void initVar();
	int mypopen();

	char* mplayer_cmd(int commandtype,int param1,int param2,bool output);
	int mplayer_start();
	string showOutput(int type);
	void mplayer_loadfile(char *fileaddr,int loadandplay);
	void mplayerconf();

	void startClutter();

       void timerIni();
	
public:

	cRenderer();
	~cRenderer();
	static void * threadCreate(void *ptr);
	static void * threadCreate1(void *ptr);
	static void cbtimer(unsigned int timeout, void* arg);
	void routon_get_scrub(float * duration,float *position ); 
	int routon_is_playing() ; //播放器是否正在播放 非暂停状态  
	void routon_set_scrub(int position); //seek至指定位置
	int routon_is_seekable();//是否可以seek
	void routon_pause();
	void routon_stop();
	void routon_play();
	void routon_add_to_playlist(char *location,int addorplay); //添加到播放列表并开始缓冲
	int routon_mplayer_is_running();//mplayer是否已经启动
	int routon_mplayer_start();
	void routon_mplayer_quit();
	void routon_volume_down();
	void routon_volume_up();
	void routon_set_mute();
	void routon_set_volume(int volume);

};



#endif /* _CRENDERER_H */


